#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

const char * featureECX[32] = {	"SSE3", 
								"PCLMULQDQ",
								"DTES64",
								"MONITOR",
								"DS-CPL",
								"VMX",
								"SMX",
								"EIST",
								"TM2",
								"SSSE3",
								"CNXT-ID",
								"SDBG",
								"FMA",
								"CMPXCHG16B",
								"xTPR Update Control",
								"PDCM",
								"",
								"PCID",
								"DCA",
								"SSE4.1",
								"SSE4.2",
								"x2APIC",
								"MOVBE",
								"POPCNT",
								"TSC-Deadline",
								"AESNI",
								"XSAVE",
								"OSXSAVE",
								"AVX",
								"F16C",
								"RDRAND",
								""};
	const char * featureEDX[32] = {	"FPU", 
								"VME",
								"DE",
								"PSE",
								"TSC",
								"MSR",
								"PAE",
								"MCE",
								"CX8",
								"APIC",
								"",
								"SEP",
								"MTRR",
								"PGE",
								"MCA",
								"CMOV",
								"PAT",
								"PSE-36",
								"PSN",
								"CLFSH",
								"",
								"DS",
								"ACPI",
								"MMX",
								"FXSR",
								"SSE",
								"SSE2",
								"SS",
								"HTT",
								"TM",
								"",
								"PBE"};

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

int CPUVendor(char *s)
{
	char buffer[13];
	
	
	cpuid_t info;
	cpuid(&info, 0, 0);
	u32ToStr(info.ebx, buffer);
	//printf("%.4s",buffer);
	u32ToStr(info.edx, buffer+4);
	//printf("%.4s",buffer);
	u32ToStr(info.ecx, buffer+8);
	//printf("%.4s\n",buffer);
	
	memcpy(s, &buffer, 13);
	
	
	//Return 1 if GenuineIntel, othervise return 0;
	if (
	memcmp((char *) &info.ebx, "Genu", 4) ||
	memcmp((char *) &info.edx, "ineI", 4) ||
	memcmp((char *) &info.ecx, "ntel", 4)
	) return 1;
	return 0;
	
}

void brandIndex(char *s){
	cpuid_t info;
	cpuid(&info, 1, 0);
	sprintf(s, "%d", info.eax & 0xFF);
	return;
}

void featureList(void){
	cpuid_t info;
	cpuid(&info, 1, 0);
	int i;
	for(i=0;i<32;++i){
		if((info.ecx & (uint32_t)(1<<i)))
		printf(" %s", featureECX[i]);
		
	}
	printf("\nFeature list from EDX:\n");
	for(i=0;i<32;++i){
		if((info.edx & (uint32_t)(1<<i)))
		printf(" %s", featureEDX[i]);
		
	}
	return;
	
}

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

int main(void)	//(int argc, char **argv)
{	
	char strbuf[256];
	CPUVendor(strbuf);
	printf("CPU Vendor: %.12s\n", strbuf);
	
	brandIndex(strbuf);
	printf("Brand Index: %s\n", strbuf);
	
	
	
	printf("\nFeature list from ECX: \n");
	featureList();
	
	
	
	return 0;
}