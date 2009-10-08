// http://msdn.microsoft.com/en-us/library/aa365801(VS.85).aspx
// http://msdn.microsoft.com/en-us/library/aa366306(VS.85).aspx


#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

/* Note: could also use malloc() and free() */

int __cdecl main(int argc, char **argv)
{

    /* Variables used by GetIpAddrTable */
    PMIB_IPADDRTABLE pIPAddrTable;
    DWORD dwSize = 0;
    DWORD dwRetVal = 0;
    IN_ADDR IPAddr;
    DWORD ifIndex;

    /* IPv4 address and subnet mask we will be adding */
    UINT iaIPAddress;
    UINT iaIPMask;

    /* Variables where handles to the added IP are returned */
    ULONG NTEContext = 0;
    ULONG NTEInstance = 0;

    /* Variables used to return error message */
    LPVOID lpMsgBuf;

 //   // Validate the parameters
 //   if (argc != 3) 
	//{
 //       printf("usage: %s IPAddress SubnetMask\n", argv[0]);
 //       exit(1);
 //   }

    //iaIPAddress = inet_addr(argv[1]);
    iaIPAddress = inet_addr("192.168.0.100");
    if (iaIPAddress == INADDR_NONE) 
	{
        printf("usage: %s IPAddress SubnetMask\n", argv[0]);
        exit(1);
    }

    //iaIPMask = inet_addr(argv[2]);
    iaIPMask = inet_addr("255.255.255.0");
    if (iaIPMask == INADDR_NONE) 
	{
        printf("usage: %s IPAddress SubnetMask\n", argv[0]);
        exit(1);
    }

    // Before calling AddIPAddress we use GetIpAddrTable to get an adapter to which we can add the IP.
    pIPAddrTable = (MIB_IPADDRTABLE *) MALLOC(sizeof (MIB_IPADDRTABLE));
    if (pIPAddrTable == NULL) 
	{
        printf("Error allocating memory needed to call GetIpAddrTable\n");
        exit (1);
    }
    else 
	{
        dwSize = 0;
        // Make an initial call to GetIpAddrTable to get the
        // necessary size into the dwSize variable
        if (GetIpAddrTable(pIPAddrTable, &dwSize, 0) == ERROR_INSUFFICIENT_BUFFER) 
		{
            FREE(pIPAddrTable);
            pIPAddrTable = (MIB_IPADDRTABLE *) MALLOC(dwSize);
        }

		if (pIPAddrTable == NULL) 
		{
            printf("Memory allocation failed for GetIpAddrTable\n");
            exit(1);
        }
    }
    // Make a second call to GetIpAddrTable to get the
    // actual data we want
    if ((dwRetVal = GetIpAddrTable(pIPAddrTable, &dwSize, 0)) == NO_ERROR) 
	{
		for (int i = 0; i < pIPAddrTable->dwNumEntries; ++i)
		{
			// Save the interface index to use for adding an IP address
			ifIndex = pIPAddrTable->table[i].dwIndex;
			printf("\n\tInterface Index:\t%ld\n", ifIndex);
			IPAddr.S_un.S_addr = (u_long) pIPAddrTable->table[i].dwAddr;
			printf("\tIP Address:       \t%s (%lu%)\n", inet_ntoa(IPAddr), pIPAddrTable->table[i].dwAddr);
			IPAddr.S_un.S_addr = (u_long) pIPAddrTable->table[i].dwMask;
			printf("\tSubnet Mask:      \t%s (%lu%)\n", inet_ntoa(IPAddr), pIPAddrTable->table[i].dwMask);
			IPAddr.S_un.S_addr = (u_long) pIPAddrTable->table[i].dwBCastAddr;
			printf("\tBroadCast Address:\t%s (%lu%)\n", inet_ntoa(IPAddr), pIPAddrTable->table[i].dwBCastAddr);
			printf("\tReassembly size:  \t%lu\n\n", pIPAddrTable->table[i].dwReasmSize);
		}

    } 
	else 
	{
        printf("Call to GetIpAddrTable failed with error %d.\n", dwRetVal);
    
		if (pIPAddrTable)
            FREE(pIPAddrTable);

		exit(1);
    }

    if (pIPAddrTable) 
	{
        FREE(pIPAddrTable);
        pIPAddrTable = NULL;
    }

    if ((dwRetVal = AddIPAddress(iaIPAddress,
                                 iaIPMask,
                                 ifIndex,
                                 &NTEContext, &NTEInstance)) == NO_ERROR) 
	{
        printf("\tIPv4 address %s was successfully added.\n", argv[1]);
    } 
	else 
	{
        printf("AddIPAddress failed with error: %d\n", dwRetVal);

        if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, dwRetVal, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),       // Default language
                          (LPTSTR) & lpMsgBuf, 0, NULL)) 
		{
            printf("\tError: %s", lpMsgBuf);
            LocalFree(lpMsgBuf);
            exit(1);
        }
    }

// Delete the IP we just added using the NTEContext
// variable where the handle was returned       
    if ((dwRetVal = DeleteIPAddress(NTEContext)) == NO_ERROR) 
	{
        printf("\tIPv4 address %s was successfully deleted.\n", argv[1]);
    } 
	else 
	{
        printf("\tDeleteIPAddress failed with error: %d\n", dwRetVal);
        exit(1);
    }

    exit(0);
}
