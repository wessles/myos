#include <stdbool.h>

#include <uefi.h>

#include "elf.h"
#include "../kernel/boot_params.h"

bool is_mem_descriptor_free(uint32_t type) {
    switch(type) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 7:
        case 14:
        return true;
        default:
        return false;
    }
}

int main(int argc, char ** argv) {
    // otherwise uefi might reset after 5 mins
    // 
    BS->SetWatchdogTimer(0, 0, 0, NULL);

    // read kernel file into memory
    long int size;
    char *buf;
    {
        FILE *file;
        file = fopen("\\kernel.elf", "r");
        if(file) {
            fseek(file, 0, SEEK_END);
            size = ftell(file);
            fseek(file, 0, SEEK_SET);
            buf = malloc(size + 1);
            if(!buf) {
                printf("Could not allocate memory for kernel.\n");
                return 1;
            }
            fread(buf, size, 1, file);
            fclose(file);
        } else {
            printf("Unable to open kernel executable.\n");
            return 1;
        }
    }

    // get boot params for the kernel
    boot_params_t bparam;
    {
        memset(&bparam, 0, sizeof(boot_params_t));

        // insert UEFI arguments
        {
            if(argc > 1) {
                bparam.argc = argc - 1;
                bparam.argv = (char**)malloc(argc * sizeof(char*));
                if(bparam.argv) {
                    int i = 0;
                    for(; i < bparam.argc; i++) {
                        bparam.argv[i] = (char*)malloc(strlen(argv[i + 1]) + 1);
                        if(bparam.argv[i]) {
                            strcpy(bparam.argv[i], argv[i + 1]);
                        }
                    }
                    bparam.argv[i] = NULL;
                }
            }
        }

        // get memory map
        {
            uintn_t mem_map_size;
            uintn_t map_key;
            uintn_t desc_size;

            efi_status_t status;

            // get the size
            status = BS->GetMemoryMap(&mem_map_size, NULL, &map_key, &desc_size, NULL);
            if(!mem_map_size) {
                printf("Error getting memory map.\n");
                return 1;
            }
            // malloc might split a record into 3, meaning add 4 additional records
            mem_map_size += 4 * desc_size;

            // allocate / fetch the mem map
            efi_memory_descriptor_t *mem_map = (efi_memory_descriptor_t*)malloc(mem_map_size);
            status = BS->GetMemoryMap(&mem_map_size, mem_map, &map_key, &desc_size, NULL);
            if(EFI_ERROR(status)) {
                printf("Unable to get memory map.\n");
                return 1;
            }

            const char *types[] = {             // i    free b4 exit?   after exit?
                "EfiReservedMemoryType",        // 0    no              no
                "EfiLoaderCode",                // 1    no              YES, may overwrite UEFI program
                "EfiLoaderData",                // 2    no              YES, may overwrite UEFI program
                "EfiBootServicesCode",          // 3    no              YES
                "EfiBootServicesData",          // 4    no              YES
                "EfiRuntimeServicesCode",       // 5    no              YES, unless in ACPI S1-S3 state
                "EfiRuntimeServicesData",       // 6    no              YES, unless in ACPI S1-S3 state
                "EfiConventionalMemory",        // 7    YES             YES
                "EfiUnusableMemory",            // 8    no              no
                "EfiACPIReclaimMemory",         // 9    no              YES, only after ACPI enabled
                "EfiACPIMemoryNVS",             // 10   no              YES, unless in ACPI S1-S3 state
                "EfiMemoryMappedIO",            // 11   no              no
                "EfiMemoryMappedIOPortSpace",   // 12   no              no
                "EfiPalCode",                   // 13   no              YES, unless in ACPI S1-S4 state
                "EfiPersistentMemory"           // 14   YES             YES
            };

            int num_reservations = 0;
            for(int i = 0; i < mem_map_size; i++) {
                bool free = is_mem_descriptor_free(mem_map[i].Type);
                if(free) num_reservations ++;
                /*
                if(free) {
                    printf("[ ] ");
                } else {
                    printf("[X] ");
                }
                printf("%016x %8d %02x %s\n", mem_map[i].PhysicalStart, mement->NumberOfPages, mement->Type, types[mement->Type]);
                */
            }

            mem_reservation_t *reservations = malloc(num_reservations * sizeof(mem_reservation_t));
            int k = 0;
            for(int i = 0; i < mem_map_size; i++) {
                bool free = is_mem_descriptor_free(mem_map[i].Type);
                if(!free) {
                    reservations[k].addr = mem_map[i].PhysicalStart;
                    reservations[k].size = mem_map[i].NumberOfPages;
                    k++;
                }
            }
            bparam.mem_reservations = reservations;
            bparam.num_reservations = num_reservations;
        }
    }

    // load entry point from kernel ELF
    uintptr_t entry;
    {
        Elf64_Ehdr *elf;
        Elf64_Phdr *phdr;

        elf = (Elf64_Ehdr *)buf;

        // if the header is valid
        if(!memcmp(elf->e_ident, ELFMAG, SELFMAG) &&    /* magic match? */
            elf->e_ident[EI_CLASS] == ELFCLASS64 &&     /* 64 bit? */
            elf->e_ident[EI_DATA] == ELFDATA2LSB &&     /* LSB? */
            elf->e_type == ET_EXEC &&                   /* executable object? */
            elf->e_machine == EM_MACH &&                /* architecture match? */
            elf->e_phnum > 0) {                         /* has program headers? */
                // load all segments
                int i;
                for(phdr = (Elf64_Phdr *)(buf + elf->e_phoff), i = 0;
                    i < elf->e_phnum;
                    i++, phdr = (Elf64_Phdr *)((uint8_t *)phdr + elf->e_phentsize)) {
                        if(phdr->p_type == PT_LOAD) {
                            memcpy((void*)phdr->p_vaddr, buf + phdr->p_offset, phdr->p_filesz);
                            memset((void*)(phdr->p_vaddr + phdr->p_filesz), 0, phdr->p_memsz - phdr->p_filesz);
                        }
                    }
                entry = elf->e_entry;
        } else {
            printf("Invalid ELF executable for this architecture.\n");
            return 1;
        }

        // free buffer
        free(buf);
    }

    // get graphics mode data and switch video mode
    {
        efi_guid_t gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
        efi_gop_t *gop = NULL;
        efi_status_t status = BS->LocateProtocol(&gopGuid, NULL, (void**)&gop);
        if(!EFI_ERROR(status) && gop) {
            status = gop->SetMode(gop, 0);
            ST->ConOut->Reset(ST->ConOut, 0);
            ST->StdErr->Reset(ST->StdErr, 0);
            if(EFI_ERROR(status)) {
                printf("Unable to set video mode.\n");
                return 0;
            }
            bparam.framebuffer = (unsigned int*)gop->Mode->FrameBufferBase;
            bparam.width = gop->Mode->Information->HorizontalResolution;
            bparam.height = gop->Mode->Information->VerticalResolution;
            bparam.pitch = sizeof(unsigned int) * gop->Mode->Information->PixelsPerScanLine;
        } else {
            printf("Unable to get graphics output protocol.\n");
            return 0;
        }
    }

    // link runtime services to kernel
    bparam.runtime_services = RT;

    // exit UEFI mode
    if(exit_bs()) {
        printf("Failed to exit boot mode.\n");
        return 1;
    }

    // execute kernel.elf with black magic fuckery syntax
    // (*((void(*name)(argument_t)) pointer))(&arg);
    (*((void(* __attribute__((sysv_abi)))(boot_params_t*))entry))(&bparam);

    return 0;
}