inline std::filesystem::path ProcessPath() {
    char szFileName[MAX_PATH];
    GetModuleFileNameA(NULL, szFileName, MAX_PATH);
    return std::filesystem::path(szFileName);
}
