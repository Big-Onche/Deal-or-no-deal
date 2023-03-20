#include <map>
#include "main.h"

namespace game
{
    map<string, vector<string>> readDialogues(const string &filename)
    {
        map<string, vector<string>> dialogs;
        ifstream file(filename);

        if(!file) fatal("Cannot read config file! (" + filename + ")");

        string line;
        string currentGroup;

        while(getline(file, line))
        {
            if(line.empty()) continue;
            if(line[0] == '[')
            {
                currentGroup = line.substr(1, line.find(']') - 1);
                dialogs[currentGroup] = vector<string>();
            }
            else dialogs[currentGroup].push_back(line);
        }
        file.close();
        return dialogs;
    }

    vector<string> introDialog;
    vector<string> chooseBoxDialog;
    vector<string> earlyGameWinDialog;
    vector<string> earlyGameMidDialog;
    vector<string> earlyGameLossDialog;
    vector<string> midGameWinDialog;
    vector<string> midGameMidDialog;
    vector<string> midGameLossDialog;
    vector<string> endGameWinDialog;
    vector<string> endGameMidDialog;
    vector<string> endGameLossDialog;
    vector<string> bankerCall;

    void loadDialogs()
    {
        map<string, vector<string>> dialogs = readDialogues("config/dialogs.cfg");

        introDialog = dialogs["introDialog"];
        chooseBoxDialog = dialogs["chooseBoxDialog"];
        earlyGameWinDialog = dialogs["earlyGameWinDialog"];
        earlyGameMidDialog = dialogs["earlyGameMidDialog"];
        earlyGameLossDialog = dialogs["earlyGameLossDialog"];
        midGameWinDialog = dialogs["midGameWinDialog"];
        midGameMidDialog = dialogs["midGameMidDialog"];
        midGameLossDialog = dialogs["midGameLossDialog"];
        endGameWinDialog = dialogs["endGameWinDialog"];
        endGameMidDialog = dialogs["endGameMidDialog"];
        endGameLossDialog = dialogs["endGameLossDialog"];
        bankerCall = dialogs["bankerCall"];
    }
}
