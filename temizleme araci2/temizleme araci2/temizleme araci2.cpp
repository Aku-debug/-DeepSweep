#include <iostream>
#include <vector>
#include <string>
#include <windows.h>
#include <tlhelp32.h>
#include <shlwapi.h>
#include <shlobj.h>
#include <algorithm>
#include <fstream>
#include <cstdlib>
#include <limits>

#pragma comment(lib, "shlwapi.lib")

using namespace std;

struct ApplicationInfo {
    string name;
    string publisher;
    string installLocation;
    string uninstallString;
    string displayVersion;
};

vector<ApplicationInfo> GetInstalledApplications() {
    vector<ApplicationInfo> apps;
    HKEY hKey;
    const int MAX_KEY_LENGTH = 255;
    char subKey[MAX_KEY_LENGTH];

    // 32-bit applications
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall",
        0, KEY_READ | KEY_WOW64_32KEY, &hKey) == ERROR_SUCCESS) {
        for (DWORD i = 0; ; i++) {
            DWORD subKeyLength = MAX_KEY_LENGTH;
            if (RegEnumKeyExA(hKey, i, subKey, &subKeyLength, NULL, NULL, NULL, NULL) != ERROR_SUCCESS)
                break;

            HKEY hSubKey;
            if (RegOpenKeyExA(hKey, subKey, 0, KEY_READ, &hSubKey) == ERROR_SUCCESS) {
                ApplicationInfo app;
                char buffer[MAX_PATH];
                DWORD bufferSize = sizeof(buffer);

                if (RegQueryValueExA(hSubKey, "DisplayName", NULL, NULL, (LPBYTE)buffer, &bufferSize) == ERROR_SUCCESS) {
                    app.name = buffer;

                    bufferSize = sizeof(buffer);
                    if (RegQueryValueExA(hSubKey, "Publisher", NULL, NULL, (LPBYTE)buffer, &bufferSize) == ERROR_SUCCESS)
                        app.publisher = buffer;

                    bufferSize = sizeof(buffer);
                    if (RegQueryValueExA(hSubKey, "InstallLocation", NULL, NULL, (LPBYTE)buffer, &bufferSize) == ERROR_SUCCESS)
                        app.installLocation = buffer;

                    bufferSize = sizeof(buffer);
                    if (RegQueryValueExA(hSubKey, "UninstallString", NULL, NULL, (LPBYTE)buffer, &bufferSize) == ERROR_SUCCESS)
                        app.uninstallString = buffer;

                    bufferSize = sizeof(buffer);
                    if (RegQueryValueExA(hSubKey, "DisplayVersion", NULL, NULL, (LPBYTE)buffer, &bufferSize) == ERROR_SUCCESS)
                        app.displayVersion = buffer;

                    if (!app.installLocation.empty()) {
                        apps.push_back(app);
                    }
                }
                RegCloseKey(hSubKey);
            }
        }
        RegCloseKey(hKey);
    }

    // 64-bit applications
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall",
        0, KEY_READ | KEY_WOW64_64KEY, &hKey) == ERROR_SUCCESS) {
        for (DWORD i = 0; ; i++) {
            DWORD subKeyLength = MAX_KEY_LENGTH;
            if (RegEnumKeyExA(hKey, i, subKey, &subKeyLength, NULL, NULL, NULL, NULL) != ERROR_SUCCESS)
                break;

            HKEY hSubKey;
            if (RegOpenKeyExA(hKey, subKey, 0, KEY_READ, &hSubKey) == ERROR_SUCCESS) {
                ApplicationInfo app;
                char buffer[MAX_PATH];
                DWORD bufferSize = sizeof(buffer);

                if (RegQueryValueExA(hSubKey, "DisplayName", NULL, NULL, (LPBYTE)buffer, &bufferSize) == ERROR_SUCCESS) {
                    app.name = buffer;

                    bufferSize = sizeof(buffer);
                    if (RegQueryValueExA(hSubKey, "Publisher", NULL, NULL, (LPBYTE)buffer, &bufferSize) == ERROR_SUCCESS)
                        app.publisher = buffer;

                    bufferSize = sizeof(buffer);
                    if (RegQueryValueExA(hSubKey, "InstallLocation", NULL, NULL, (LPBYTE)buffer, &bufferSize) == ERROR_SUCCESS)
                        app.installLocation = buffer;

                    bufferSize = sizeof(buffer);
                    if (RegQueryValueExA(hSubKey, "UninstallString", NULL, NULL, (LPBYTE)buffer, &bufferSize) == ERROR_SUCCESS)
                        app.uninstallString = buffer;

                    bufferSize = sizeof(buffer);
                    if (RegQueryValueExA(hSubKey, "DisplayVersion", NULL, NULL, (LPBYTE)buffer, &bufferSize) == ERROR_SUCCESS)
                        app.displayVersion = buffer;

                    // Check if this app is already in the list
                    bool exists = false;
                    for (const auto& existingApp : apps) {
                        if (existingApp.name == app.name && existingApp.installLocation == app.installLocation) {
                            exists = true;
                            break;
                        }
                    }

                    if (!exists && !app.installLocation.empty()) {
                        apps.push_back(app);
                    }
                }
                RegCloseKey(hSubKey);
            }
        }
        RegCloseKey(hKey);
    }

    // Current user applications
    if (RegOpenKeyExA(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall",
        0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        for (DWORD i = 0; ; i++) {
            DWORD subKeyLength = MAX_KEY_LENGTH;
            if (RegEnumKeyExA(hKey, i, subKey, &subKeyLength, NULL, NULL, NULL, NULL) != ERROR_SUCCESS)
                break;

            HKEY hSubKey;
            if (RegOpenKeyExA(hKey, subKey, 0, KEY_READ, &hSubKey) == ERROR_SUCCESS) {
                ApplicationInfo app;
                char buffer[MAX_PATH];
                DWORD bufferSize = sizeof(buffer);

                if (RegQueryValueExA(hSubKey, "DisplayName", NULL, NULL, (LPBYTE)buffer, &bufferSize) == ERROR_SUCCESS) {
                    app.name = buffer;

                    bufferSize = sizeof(buffer);
                    if (RegQueryValueExA(hSubKey, "Publisher", NULL, NULL, (LPBYTE)buffer, &bufferSize) == ERROR_SUCCESS)
                        app.publisher = buffer;

                    bufferSize = sizeof(buffer);
                    if (RegQueryValueExA(hSubKey, "InstallLocation", NULL, NULL, (LPBYTE)buffer, &bufferSize) == ERROR_SUCCESS)
                        app.installLocation = buffer;

                    bufferSize = sizeof(buffer);
                    if (RegQueryValueExA(hSubKey, "UninstallString", NULL, NULL, (LPBYTE)buffer, &bufferSize) == ERROR_SUCCESS)
                        app.uninstallString = buffer;

                    bufferSize = sizeof(buffer);
                    if (RegQueryValueExA(hSubKey, "DisplayVersion", NULL, NULL, (LPBYTE)buffer, &bufferSize) == ERROR_SUCCESS)
                        app.displayVersion = buffer;

                    if (!app.installLocation.empty()) {
                        apps.push_back(app);
                    }
                }
                RegCloseKey(hSubKey);
            }
        }
        RegCloseKey(hKey);
    }

    return apps;
}

void ScanForPortableApps(const string& drive, vector<ApplicationInfo>& apps) {
    WIN32_FIND_DATAA findFileData;
    HANDLE hFind = INVALID_HANDLE_VALUE;

    vector<string> commonFolders = {
        "Program Files", "Program Files (x86)", "ProgramData", "Users", "AppData"
    };

    for (const auto& folder : commonFolders) {
        string searchPath = drive + "\\" + folder + "\\*";
        hFind = FindFirstFileA(searchPath.c_str(), &findFileData);

        if (hFind != INVALID_HANDLE_VALUE) {
            do {
                if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                    string dirName = findFileData.cFileName;
                    if (dirName != "." && dirName != "..") {
                        // Check for executable files
                        string exeSearchPath = drive + "\\" + folder + "\\" + dirName + "\\*.exe";
                        HANDLE hExeFind = FindFirstFileA(exeSearchPath.c_str(), &findFileData);

                        if (hExeFind != INVALID_HANDLE_VALUE) {
                            ApplicationInfo app;
                            app.name = dirName;
                            app.installLocation = drive + "\\" + folder + "\\" + dirName;
                            app.publisher = "Portable Application";

                            // Check if this app is already in the list
                            bool exists = false;
                            for (const auto& existingApp : apps) {
                                if (existingApp.name == app.name && existingApp.installLocation == app.installLocation) {
                                    exists = true;
                                    break;
                                }
                            }

                            if (!exists) {
                                apps.push_back(app);
                            }

                            FindClose(hExeFind);
                        }
                    }
                }
            } while (FindNextFileA(hFind, &findFileData) != 0);
            FindClose(hFind);
        }
    }
}

vector<ApplicationInfo> GetAllApplications() {
    vector<ApplicationInfo> apps = GetInstalledApplications();

    // Get all drives
    DWORD drives = GetLogicalDrives();
    for (char drive = 'A'; drive <= 'Z'; drive++) {
        if (drives & (1 << (drive - 'A'))) {
            string drivePath = string(1, drive) + ":";
            ScanForPortableApps(drivePath, apps);
        }
    }

    return apps;
}

bool UninstallApplication(const ApplicationInfo& app) {
    // First try the standard uninstall string
    if (!app.uninstallString.empty()) {
        string command = app.uninstallString;

        // Some uninstall strings have quotes, some don't
        if (command.find("\"") == string::npos) {
            size_t spacePos = command.find(" ");
            if (spacePos != string::npos) {
                string path = command.substr(0, spacePos);
                string args = command.substr(spacePos + 1);
                command = "\"" + path + "\" " + args;
            }
        }

        // Add silent uninstall parameters if possible
        if (command.find("/quiet") == string::npos && command.find("-quiet") == string::npos &&
            command.find("/S") == string::npos && command.find("-s") == string::npos) {
            if (command.find("msiexec") != string::npos) {
                command += " /quiet /norestart";
            }
            else {
                command += " /S"; // Common silent uninstall parameter
            }
        }

        STARTUPINFOA si = { sizeof(si) };
        PROCESS_INFORMATION pi;

        if (CreateProcessA(NULL, (LPSTR)command.c_str(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
            WaitForSingleObject(pi.hProcess, INFINITE);
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);

            // Wait a bit to let the uninstaller complete
            Sleep(5000);
        }
    }

    // Then delete the installation directory
    if (!app.installLocation.empty() && PathFileExistsA(app.installLocation.c_str())) {
        // First try to remove the directory normally
        SHFILEOPSTRUCTA fileOp = { 0 };
        fileOp.hwnd = NULL;
        fileOp.wFunc = FO_DELETE;

        // Double null-terminated string
        string path = app.installLocation + '\0' + '\0';
        fileOp.pFrom = path.c_str();
        fileOp.pTo = NULL;
        fileOp.fFlags = FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_SILENT;

        SHFileOperationA(&fileOp);

        // If still exists, try again after a delay
        if (PathFileExistsA(app.installLocation.c_str())) {
            Sleep(3000); // Wait 3 seconds
            SHFileOperationA(&fileOp);
        }
    }

    // Remove registry entries
    HKEY hKey;
    const int MAX_KEY_LENGTH = 255;
    char subKey[MAX_KEY_LENGTH];

    // Check 32-bit uninstall keys
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall",
        0, KEY_READ | KEY_WRITE | KEY_WOW64_32KEY, &hKey) == ERROR_SUCCESS) {
        for (DWORD i = 0; ; i++) {
            DWORD subKeyLength = MAX_KEY_LENGTH;
            if (RegEnumKeyExA(hKey, i, subKey, &subKeyLength, NULL, NULL, NULL, NULL) != ERROR_SUCCESS)
                break;

            HKEY hSubKey;
            if (RegOpenKeyExA(hKey, subKey, 0, KEY_READ, &hSubKey) == ERROR_SUCCESS) {
                char buffer[MAX_PATH];
                DWORD bufferSize = sizeof(buffer);

                if (RegQueryValueExA(hSubKey, "DisplayName", NULL, NULL, (LPBYTE)buffer, &bufferSize) == ERROR_SUCCESS) {
                    if (string(buffer) == app.name) {
                        RegCloseKey(hSubKey);
                        RegDeleteKeyA(hKey, subKey);
                        break;
                    }
                }
                RegCloseKey(hSubKey);
            }
        }
        RegCloseKey(hKey);
    }

    // Check 64-bit uninstall keys
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall",
        0, KEY_READ | KEY_WRITE | KEY_WOW64_64KEY, &hKey) == ERROR_SUCCESS) {
        for (DWORD i = 0; ; i++) {
            DWORD subKeyLength = MAX_KEY_LENGTH;
            if (RegEnumKeyExA(hKey, i, subKey, &subKeyLength, NULL, NULL, NULL, NULL) != ERROR_SUCCESS)
                break;

            HKEY hSubKey;
            if (RegOpenKeyExA(hKey, subKey, 0, KEY_READ, &hSubKey) == ERROR_SUCCESS) {
                char buffer[MAX_PATH];
                DWORD bufferSize = sizeof(buffer);

                if (RegQueryValueExA(hSubKey, "DisplayName", NULL, NULL, (LPBYTE)buffer, &bufferSize) == ERROR_SUCCESS) {
                    if (string(buffer) == app.name) {
                        RegCloseKey(hSubKey);
                        RegDeleteKeyA(hKey, subKey);
                        break;
                    }
                }
                RegCloseKey(hSubKey);
            }
        }
        RegCloseKey(hKey);
    }

    // Check current user uninstall keys
    if (RegOpenKeyExA(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall",
        0, KEY_READ | KEY_WRITE, &hKey) == ERROR_SUCCESS) {
        for (DWORD i = 0; ; i++) {
            DWORD subKeyLength = MAX_KEY_LENGTH;
            if (RegEnumKeyExA(hKey, i, subKey, &subKeyLength, NULL, NULL, NULL, NULL) != ERROR_SUCCESS)
                break;

            HKEY hSubKey;
            if (RegOpenKeyExA(hKey, subKey, 0, KEY_READ, &hSubKey) == ERROR_SUCCESS) {
                char buffer[MAX_PATH];
                DWORD bufferSize = sizeof(buffer);

                if (RegQueryValueExA(hSubKey, "DisplayName", NULL, NULL, (LPBYTE)buffer, &bufferSize) == ERROR_SUCCESS) {
                    if (string(buffer) == app.name) {
                        RegCloseKey(hSubKey);
                        RegDeleteKeyA(hKey, subKey);
                        break;
                    }
                }
                RegCloseKey(hSubKey);
            }
        }
        RegCloseKey(hKey);
    }

    // Remove from AppData
    char appDataPath[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, appDataPath))) {
        string appDataDir = string(appDataPath) + "\\" + app.name;
        if (PathFileExistsA(appDataDir.c_str())) {
            SHFILEOPSTRUCTA fileOp = { 0 };
            fileOp.hwnd = NULL;
            fileOp.wFunc = FO_DELETE;

            string path = appDataDir + '\0' + '\0';
            fileOp.pFrom = path.c_str();
            fileOp.pTo = NULL;
            fileOp.fFlags = FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_SILENT;

            SHFileOperationA(&fileOp);
        }
    }

    // Remove from LocalAppData
    char localAppDataPath[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, localAppDataPath))) {
        string localAppDataDir = string(localAppDataPath) + "\\" + app.name;
        if (PathFileExistsA(localAppDataDir.c_str())) {
            SHFILEOPSTRUCTA fileOp = { 0 };
            fileOp.hwnd = NULL;
            fileOp.wFunc = FO_DELETE;

            string path = localAppDataDir + '\0' + '\0';
            fileOp.pFrom = path.c_str();
            fileOp.pTo = NULL;
            fileOp.fFlags = FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_SILENT;

            SHFileOperationA(&fileOp);
        }
    }

    return true;
}

int main() {
    SetConsoleTitleA("Advanced Application Remover");

    cout << "Scanning for all installed and portable applications..." << endl;
    vector<ApplicationInfo> apps = GetAllApplications();

    if (apps.empty()) {
        cout << "No applications found." << endl;
        return 0;
    }

    // Sort applications alphabetically
    sort(apps.begin(), apps.end(), [](const ApplicationInfo& a, const ApplicationInfo& b) {
        return a.name < b.name;
        });

    // Display applications
    for (size_t i = 0; i < apps.size(); i++) {
        cout << i + 1 << ". " << apps[i].name;
        if (!apps[i].publisher.empty()) {
            cout << " (" << apps[i].publisher << ")";
        }
        if (!apps[i].displayVersion.empty()) {
            cout << " [v" << apps[i].displayVersion << "]";
        }
        cout << endl;
        cout << "   Location: " << apps[i].installLocation << endl;
        cout << endl;
    }

    // Get user selection
    int choice = 0;
    while (choice < 1 || choice > apps.size()) {
        cout << "Select an application to uninstall (1-" << apps.size() << "): ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            choice = 0;
        }
    }

    ApplicationInfo selectedApp = apps[choice - 1];

    // Confirm uninstallation
    char confirm;
    cout << "Are you sure you want to completely uninstall \"" << selectedApp.name << "\"? (y/n): ";
    cin >> confirm;

    if (tolower(confirm) == 'y') {
        cout << "Uninstalling " << selectedApp.name << "..." << endl;

        if (UninstallApplication(selectedApp)) {
            cout << "Application successfully uninstalled and all traces removed." << endl;
        }
        else {
            cout << "Failed to completely uninstall the application. Some components may remain." << endl;
        }
    }
    else {
        cout << "Uninstallation cancelled." << endl;
    }

    return 0;
}