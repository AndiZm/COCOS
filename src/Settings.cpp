#include "Settings.hpp"

Settings::Settings(){
    fSettingsMap.insert(std::make_pair("is", 0));
    fSettingsMap.insert(std::make_pair("ie", 0));
    fSettingsMap.insert(std::make_pair("sl", 0));
    fSettingsMap.insert(std::make_pair("ts", -250e3));
    fSettingsMap.insert(std::make_pair("te", 250e3));
    fSettingsMap.insert(std::make_pair("nb", 2000));
    fSettingsMap.insert(std::make_pair("cm", 0));
}

std::string Settings::GetCalibrationFileName() const
{
    return fCalibrationFile;
}

void Settings::ReadSettingsFile()
{
    std::ifstream ifs(fSettingsFile);
    if(!ifs){
        std::cerr << "ERROR: Cannot open settings file settings.txt!\n" << std::endl;
        std::cerr << "Does a settings.txt exist?" << std::endl;
    } else if (!CheckSettingsFile(fSettingsFile)){
        std::cerr << "Wrong layout of settings.txt\n" << std::endl;
    }
    std::string line;
    while(getline(ifs, line)){
        if(line[0] == '#'){
            continue;
        } else if (line[0] == '$'){
            fCalibrationFile = line.substr(1, line.size()-1);
        } 
        else {
            std::string parameter;
            long long value;
            parameter = line.substr(0,2);
            value = std::stoll(line.substr(3, line.size()-1));
            if(parameter == "is"){
                SetStartEvalTime(value);
            } else if (parameter == "ie"){
                SetEndEvalTime(value);
            } else if (parameter == "sl"){
                SetTimeLimitation(value);
            } else if (parameter == "ts"){
                SetTauBegin(value);
            } else if (parameter == "te"){
                SetTauEnd(value);
            } else if (parameter == "nb"){
                SetNumberOfBins(value);
            } else if (parameter == "cm"){
                SetCalibrationMode(value);
            }
        }
    }
    PrintSettingsFile(fSettingsFile);
}

void PrintSettingsFile(std::string file){
    std::ifstream f(file);
    std::cout << f.rdbuf();
}

bool CheckSettingsFile(std::string file){
    std::ifstream ifs(file);
    std::string line;
    while(getline(ifs, line)){
        if(line[0] == '#'){
            continue;
        } else if (line[2] != ' '){
            return false;
        }
    }
    return true;
}

void Settings::PrintSettings()
{
    std::string change;
    std::cout << "Current Settings:\n#################\n";
    std::cout << "Start evaluation time [is]:\t" << 1e-12 * fSettingsMap["is"] << " s\n";
    std::cout << "End evaluation time [ie]:\t" << 1e-12 * fSettingsMap["ie"] << " s\n";
    std::cout << "Set input limitation [sl]:\t" << fSettingsMap["sl"] << "\n";
    std::cout << "Start time difference [tb]:\t" << fSettingsMap["tb"] << " ps\n";
    std::cout << "End time difference [te]:\t" << fSettingsMap["te"] << " ps\n";
    std::cout << "Number of bins [nb]:\t\t" << fSettingsMap["nb"] << "\n";
    std::cout << "Calibration mode [cm]:\t\t" << fSettingsMap["cm"] << "\n";
    std::cout << "\nDo you want to change something (c for continue)? ";
    std::cin >> change;
    ChangeSettings(change);
}

void Settings::ChangeSettings(std::string change)
{
    // TODO: Add warnings if the values are incorrect
    // e.g. ts > te or not a number
    if (change == "is"){
        long long starteval;
        std::cout << "Enter new start evaluation time [s]: ";
        std::cin >> starteval;
        SetStartEvalTime(starteval);
    } else if (change == "ie"){
        long long endeval;
        std::cout << "Enter new start evaluation time [s]: ";
        std::cin >> endeval;
        SetEndEvalTime(endeval);
    } else if (change == "sl"){
        bool inputlimit;
        std::cout << "Enter 0 (=false) or 1 (=true): ";
        std::cin >> inputlimit;
        SetTimeLimitation((bool)inputlimit);
    } else if (change == "tb"){
        int taubegin;
        std::cout << "Enter new start time difference [ns]: ";
        std::cin >> taubegin;
        SetTauBegin(taubegin);
    } else if (change == "te"){
        int tauend;
        std::cout << "Enter new start time difference [ns]: ";
        std::cin >> tauend;
        SetTauEnd(tauend);
    } else if (change == "nb"){
        int numofbins;
        std::cout << "Enter new number of bins: ";
        std::cin >> numofbins;
        SetNumberOfBins(numofbins);
    } else if (change == "cm"){
        int calibrationmode;
        std::cout << "Enter 1 (=read), 2 (=write) or 0 (=none): ";
        std::cin >> calibrationmode;
        SetCalibrationMode(calibrationmode);
    } else if (change == "c"){
        goto endmarker;
    } else {
        std::cout << "Wrong parameter!" << std::endl;
    }
    PrintSettings();
    endmarker:;
}

int Settings::GetNumberOfBins() const
{
    return (int)(fSettingsMap.find("nb") -> second);
}

int Settings::GetTauBegin() const
{
    return (int)(fSettingsMap.find("tb") -> second);
}

int Settings::GetTauEnd() const
{
    return (int)(fSettingsMap.find("te") -> second);
}

int Settings::GetCalibrationMode() const
{
    return (int)(fSettingsMap.find("cm") -> second);
}

long long Settings::GetStartEvalTime() const
{
    return (long long)(fSettingsMap.find("is") -> second);
}

long long Settings::GetEndEvalTime() const
{
    return (long long)(fSettingsMap.find("ie") -> second);
}

bool Settings::GetTimeLimitation() const
{
    return (bool)(fSettingsMap.find("sl") -> second);
}

void Settings::SetCalibrationMode(int calibrationmode)
{
    fSettingsMap["cm"] = calibrationmode;
}

void Settings::SetStartEvalTime(long long starteval)
{
    fSettingsMap["is"] = 1e12 * starteval;
}

void Settings::SetEndEvalTime(long long endeval)
{
    fSettingsMap["ie"] = 1e12 * endeval;
}

void Settings::SetTimeLimitation(bool inputlimit)
{
    fSettingsMap["sl"] = inputlimit;
}

void Settings::SetTauBegin(int taubegin)
{
    fSettingsMap["tb"] = taubegin;
}

void Settings::SetTauEnd(int tauend)
{
    fSettingsMap["te"] = tauend;
}

void Settings::SetNumberOfBins(int numofbins)
{
    fSettingsMap["nb"] = numofbins;
}
