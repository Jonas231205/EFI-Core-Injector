#include <efi.h>
#include <efilib.h>

// Define the address for ICR (Interrupt Command Register) in Local APIC
#define APIC_ICR_ADDRESS 0xFEE00300
#define GNU_EFI_USE_MS_ABI 1
#define COMMAND_MAGIC baseOperation*0x7346

// Function to introduce a delay
void delay(UINTN microseconds) {
    // Estimate number of iterations required for the specified delay
    // This estimate might need adjustment depending on the processor speed. 100000 is about 1 sec
    UINTN iterations = microseconds * 1000;

    // Simple loop to introduce a delay
    for (UINTN i = 0; i < iterations; i++) {
        __asm__ volatile ("nop");
    }
}

// EFI entry point
EFI_STATUS 
efi_main(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable) {
    // Initialize internal GNU-EFI functions
    InitializeLib(ImageHandle, SystemTable);

    // Get handle to this image
    EFI_LOADED_IMAGE *LoadedImage = NULL;
    EFI_STATUS status = uefi_call_wrapper(BS->OpenProtocol, 6, ImageHandle, &gEfiLoadedImageProtocolGuid,
                                        (VOID**)&LoadedImage, ImageHandle, NULL, EFI_OPEN_PROTOCOL_GET_PROTOCOL);
	/* Function to TODO:
		1. Detect number of physical processor
		
		2. Replace the variable to Number of core then sending IPI Instructions.
		
		3. Windows partition finder
		
		4. Bcd data load and config number of proc check and changer.
		
		5. Detect Number of RAM and setting max memory on windows os.
		
		6. Start Address for the processor setting to the start address of the blocked list from bcd data.
			
		7. Map some code to the specific start address.
		
		8. After mapping unload the driver and call windows bootloader.
		
		*/
		
		
		__asm__ volatile (
    "movq $0x2000, %%rdi\n"  // Adresse til skrivning (ændr til din ønskede adresse)
    "movl $0xCD19, %%eax\n"       // Værdi der skal skrives (ændr til din ønskede værdi)
    "movl %%eax, (%%rdi)\n"       // Skriv værdien til den angivne adresse
    :
    :
    : "eax", "rdi"
);


    // Send IPI to the target processor.
    UINT32 StartAddress = 0x2000; // Start address for the 6th processor.  // Angiv den ønskede processor-ID (6)
UINT8 ProcessorId = 6;

// Use the variables to define Processor config.
UINT32 IpiCommand = (ProcessorId << 24) | 0x4600 + (StartAddress >> 12);

// Send IPI
__asm__ volatile (
    "movq $0xFEE00300, %%rsi\n"      // Loading address for ICR i RSI
    "movl %0, %%eax\n"                // Loading IPI-Commando in EAX
    "movl %%eax, (%%rsi)\n"           // Send IPI-Commando
    :
    : "r" (IpiCommand)
    : "eax", "esi"
);


    // Introduce a delay of 1000 milliseconds (adjust as needed)
    delay(100000);

    // Print "Hello, World!"
    Print(L"Hello, World!\n");

    // Return to the calling shell
    uefi_call_wrapper(BS->Exit, 4, ImageHandle, EFI_SUCCESS, 0, NULL);

    // This point should never be reached
    return EFI_SUCCESS;
}
