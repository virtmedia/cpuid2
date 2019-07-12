#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct cpuid_struct {
	unsigned int eax;
	unsigned int ebx;
	unsigned int ecx;
	unsigned int edx;
} cpuid_t;

void cpuid (cpuid_t *info, unsigned int leaf, unsigned int subleaf)
{
	asm volatile("cpuid"
	: "=a" (info->eax), "=b" (info->ebx), "=c" (info->ecx), "=d" (info->edx)
	: "a" (leaf), "c" (subleaf)
	);
}

void u32ToStr (uint32_t value, char *buffer)
{
	for(int i=0; i<4; i++)
	{
		buffer[i] = (value & 0xFF);
		value = (value >> 8);
	}
	buffer[4] = 0;
}

int printCPUVendor(void)
{
	char buffer[8];
	cpuid_t info;
	cpuid(&info, 0, 0);
	u32ToStr(info.ebx, buffer);
	printf("%.4s",buffer);
	u32ToStr(info.edx, buffer);
	printf("%.4s",buffer);
	u32ToStr(info.ecx, buffer);
	printf("%.4s\n",buffer);
	//Return 1 if GenuineIntel, othervise return 0;
	if (
	memcmp((char *) &info.ebx, "Genu", 4) ||
	memcmp((char *) &info.edx, "ineI", 4) ||
	memcmp((char *) &info.ecx, "ntel", 4)
	) return 1;
	return 0;
	
}




int main(void)	//(int argc, char **argv)
{	
	int isIntel = printCPUVendor();
	cpuid_t info;
	cpuid(&info, 1, 0);
	/*
		EAX
			Version Information: Type, Family, Model, and Stepping ID (see Figure 3-6)
		EBX
			Bits 07-00: Brand Index
			Bits 15-08: CLFLUSH line size (Value ∗ 8 = cache line size in bytes)
			Bits 23-16: Maximum number of addressable IDs for logical processors in this physical package*.
			Bits 31-24: Initial APIC ID
		ECX
			Feature Information (see Figure 3-7 and Table 3-19)
		EDX
			Feature Information (see Figure 3-8 and Table 3-20)
		
		NOTES:
			* The nearest power-of-2 integer that is not smaller than EBX[23:16] is the number of unique initial APIC
			IDs reserved for addressing different logical processors in a physical package. This field is only valid if
			CPUID.1.EDX.HTT[bit 28]= 1.
	*/
	printf("Brand Index: %d\n", info.eax & 0xFF);
	
	printf("Feature list:\n");
	if((info.ecx & (uint32_t)(1<<0)))
		printf("\tSSE3\n");
	if((info.ecx & (uint32_t)(1<<1)))
		printf("\tPCLMULQDQ\n");
	if((info.ecx & (uint32_t)(1<<2)))
		printf("\tDTES64\n");
	if((info.ecx & (uint32_t)(1<<3)))
		printf("\tMONITOR\n");
	if((info.ecx & (uint32_t)(1<<4)))
		printf("\tDS-CPL\n");
	if((info.ecx & (uint32_t)(1<<5)))
		printf("\tVMX\n");
	if((info.ecx & (uint32_t)(1<<6)))
		printf("\tSMX\n");
	if((info.ecx & (uint32_t)(1<<7)))
		printf("\tEIST\n");
	if((info.ecx & (uint32_t)(1<<8)))
		printf("\tTM2\n");
	if((info.ecx & (uint32_t)(1<<9)))
		printf("\tSSSE3\n");
	if((info.ecx & (uint32_t)(1<<10)))
		printf("\tCNXT-ID\n");
	if((info.ecx & (uint32_t)(1<<11)))
		printf("\tSDBG\n");
	if((info.ecx & (uint32_t)(1<<12)))
		printf("\tFMA\n");
	if((info.ecx & (uint32_t)(1<<13)))
		printf("\tCMPXCHG16B\n");
	if((info.ecx & (uint32_t)(1<<14)))
		printf("\txTPR Update Control\n");
	if((info.ecx & (uint32_t)(1<<15)))
		printf("\tPDCM\n");
	if((info.ecx & (uint32_t)(1<<17)))
		printf("\tPCID\n");
	if((info.ecx & (uint32_t)(1<<18)))
		printf("\tDCA\n");
	if((info.ecx & (uint32_t)(1<<19)))
		printf("\tSSE4.1\n");
	if((info.ecx & (uint32_t)(1<<20)))
		printf("\tSSE4.2\n");
	if((info.ecx & (uint32_t)(1<<21)))
		printf("\tx2APIC\n");
	if((info.ecx & (uint32_t)(1<<22)))
		printf("\tMOVBE\n");
	if((info.ecx & (uint32_t)(1<<23)))
		printf("\tPOPCNT\n");
	if((info.ecx & (uint32_t)(1<<24)))
		printf("\tTSC-Deadline\n");
	if((info.ecx & (uint32_t)(1<<25)))
		printf("\tAESNI\n");
	if((info.ecx & (uint32_t)(1<<26)))
		printf("\tXSAVE\n");
	if((info.ecx & (uint32_t)(1<<27)))
		printf("\tOSXSAVE\n");
	if((info.ecx & (uint32_t)(1<<28)))
		printf("\tAVX\n");
	if((info.ecx & (uint32_t)(1<<29)))
		printf("\tF16C\n");
	if((info.ecx & (uint32_t)(1<<30)))
		printf("\tRDRAND\n");
	
	if((info.edx & (uint32_t)(1<<0)))
		printf("\tFPU\n");
	if((info.edx & (uint32_t)(1<<1)))
		printf("\tVME\n");
	if((info.edx & (uint32_t)(1<<2)))
		printf("\tDE\n");
	if((info.edx & (uint32_t)(1<<3)))
		printf("\tPSE\n");
	if((info.edx & (uint32_t)(1<<4)))
		printf("\tTSC\n");
	if((info.edx & (uint32_t)(1<<5)))
		printf("\tMSR\n");
	if((info.edx & (uint32_t)(1<<6)))
		printf("\tPAE\n");
	if((info.edx & (uint32_t)(1<<7)))
		printf("\tMCE\n");
	if((info.edx & (uint32_t)(1<<8)))
		printf("\tCX8\n");
	if((info.edx & (uint32_t)(1<<9)))
		printf("\tAPIC\n");
	if((info.edx & (uint32_t)(1<<11)))
		printf("\tSEP\n");
	if((info.edx & (uint32_t)(1<<12)))
		printf("\tMTRR\n");
	if((info.edx & (uint32_t)(1<<13)))
		printf("\tPGE\n");
	if((info.edx & (uint32_t)(1<<14)))
		printf("\tMCA\n");
	if((info.edx & (uint32_t)(1<<15)))
		printf("\tCMOV\n");
	if((info.edx & (uint32_t)(1<<16)))
		printf("\tPAT\n");
	if((info.edx & (uint32_t)(1<<17)))
		printf("\tPSE-36\n");
	if((info.edx & (uint32_t)(1<<18)))
		printf("\tPSN\n");
	if((info.edx & (uint32_t)(1<<19)))
		printf("\tCLFSH\n");
	if((info.edx & (uint32_t)(1<<21)))
		printf("\tDS\n");
	if((info.edx & (uint32_t)(1<<22)))
		printf("\tACPI\n");
	if((info.edx & (uint32_t)(1<<23)))
		printf("\tMMX\n");
	if((info.edx & (uint32_t)(1<<24)))
		printf("\tFXSR\n");
	if((info.edx & (uint32_t)(1<<25)))
		printf("\tSSE\n");
	if((info.edx & (uint32_t)(1<<26)))
		printf("\tSSE2\n");
	if((info.edx & (uint32_t)(1<<27)))
		printf("\tSS\n");
	if((info.edx & (uint32_t)(1<<28)))
		printf("\tHTT\n");
	if((info.edx & (uint32_t)(1<<29)))
		printf("\tTM\n");
	if((info.edx & (uint32_t)(1<<31)))
		printf("\tPBE\n");
	
	
	return 0;
}