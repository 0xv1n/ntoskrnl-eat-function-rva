#include <iostream>
#include <Windows.h>

bool FindFunctionInEAT(const std::wstring& moduleName, const std::string& functionName) {
    HMODULE moduleHandle = LoadLibraryEx(moduleName.c_str(), NULL, DONT_RESOLVE_DLL_REFERENCES);
    if (!moduleHandle) {
        std::cerr << "Failed to load module." << std::endl;
        return false;
    }

    PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)moduleHandle;
    PIMAGE_NT_HEADERS ntHeaders = (PIMAGE_NT_HEADERS)((BYTE*)moduleHandle + dosHeader->e_lfanew);
    PIMAGE_EXPORT_DIRECTORY exportDir = (PIMAGE_EXPORT_DIRECTORY)((BYTE*)moduleHandle + ntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
    DWORD* functions = (DWORD*)((BYTE*)moduleHandle + exportDir->AddressOfFunctions);
    DWORD* names = (DWORD*)((BYTE*)moduleHandle + exportDir->AddressOfNames);
    WORD* ordinals = (WORD*)((BYTE*)moduleHandle + exportDir->AddressOfNameOrdinals);

    for (DWORD i = 0; i < exportDir->NumberOfNames; i++) {
        char* funcName = (char*)((BYTE*)moduleHandle + names[i]);
        if (functionName == funcName) {
            std::cout << functionName << " found at RVA: " << std::hex << functions[ordinals[i]] << std::endl;
            FreeLibrary(moduleHandle);
            return true;
        }
    }

    FreeLibrary(moduleHandle);
    return false;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <FunctionName>" << std::endl;
        return 1;
    }

    std::string functionName = argv[1];

    if (!FindFunctionInEAT(L"C:\\Windows\\System32\\ntoskrnl.exe", functionName)) {
        std::cerr << "Function not found." << std::endl;
        return 1;
    }

    return 0;
}
