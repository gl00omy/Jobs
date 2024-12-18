// CPULimit.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <stdio.h>

int Error(const char* msg) {
    printf("%s (%u)\n", msg, GetLastError());
    return 1;
}
int main(int argc, const char* argv[])
{
    if (argc < 3) {
        printf("Usage: CPULimit <pid> <percent>\n");
        return 0; 
    }
    int pid = atoi(argv[1]);
    int percent = atoi(argv[2]);

    HANDLE hJob = CreateJobObject(nullptr, L"CpuLimitJob");
    if (!hJob)
        return Error("Error creating job");

    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE | PROCESS_SET_QUOTA, FALSE, pid);
    if (!hProcess)
        return Error("Error in OpenProcess");

    if (!AssignProcessToJobObject(hJob, hProcess))
        return Error("Error in assigning process");

    CloseHandle(hProcess);

    JOBOBJECT_CPU_RATE_CONTROL_INFORMATION info;
    info.ControlFlags = JOB_OBJECT_CPU_RATE_CONTROL_ENABLE | JOB_OBJECT_CPU_RATE_CONTROL_HARD_CAP;
    info.CpuRate = percent * 100;

    if (!SetInformationJobObject(hJob, JobObjectCpuRateControlInformation, &info, sizeof(info)))
        return Error("Failed to set limit");

    printf("Success!\n");

    Sleep(INFINITE);  
    return 0;
}


