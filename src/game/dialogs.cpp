#include "game.h"
#include "main.h"
#include <optional>

namespace game
{
    map<DialogueType, vector<string>> dialogues;

    DialogueType dialogueTypeFromString(const string &str) {
        static const map<string, DialogueType> mapping = {
            {"GAME_INTRO", DialogueType::Intro},
            {"CHOOSE_BOX", DialogueType::ChooseBox},
            {"EARLY_GAME_WIN", DialogueType::EarlyGameWin},
            {"EARLY_GAME_MID", DialogueType::EarlyGameMid},
            {"EARLY_GAME_LOSS", DialogueType::EarlyGameLoss},
            {"MID_GAME_WIN", DialogueType::MidGameWin},
            {"MID_GAME_MID", DialogueType::MidGameMid},
            {"MID_GAME_LOSS", DialogueType::MidGameLoss},
            {"END_GAME_WIN", DialogueType::EndGameWin},
            {"END_GAME_MID", DialogueType::EndGameMid},
            {"END_GAME_LOSS", DialogueType::EndGameLoss},
            {"BANKER_CALL", DialogueType::BankerCall},
            {"DEAL_REFUSED", DialogueType::DealRefused},
        };
        auto it = mapping.find(str);
        if (it != mapping.end()) {
            return it->second;
        }
        logoutf("Warning Invalid dialogue type string (%s)", str.c_str());
        return DialogueType::Invalid;
    }

    void readDialogues(const string &filename, map<DialogueType, vector<string>> &dialogues)
    {
        ifstream file(filename);

        if (!file) fatal("Cannot read config file! (" + filename + ")");

        string line;
        DialogueType currentGroup;

        while (getline(file, line))
        {
            if (line.empty()) continue;
            if (line[0] == '[')
            {
                currentGroup = dialogueTypeFromString(line.substr(1, line.find(']') - 1));
                dialogues[currentGroup] = vector<string>();
            }
            else dialogues[currentGroup].push_back(line);
        }
        file.close();
    }

    void loadDialogs()
    {
        readDialogues("config/dialogs.cfg", dialogues);
    }

    const string &getRandomDialogue(DialogueType type)
    {
        auto it = dialogues.find(type);
        if(it == dialogues.end())
        {
            static const string defaultString = "Error: Dialogue not found (and I'm not kidding)";
            return defaultString;
        }
        auto &dialogueVec = it->second;
        int randomIndex = rnd(dialogueVec.size());
        return dialogueVec[randomIndex];
    }

    string mainDialog;
    void popDialog(const char* format, ...)
    {
        char buffer[256];
        va_list args;
        va_start(args, format);
        vsnprintf(buffer, 256, format, args);
        va_end(args);

        if(strcmp(mainDialog.c_str(), buffer)) // avoid rewriting the same text in cases where it happen
        {
            mainDialog = buffer;
            lastDialogTime = elapsedTime;
        }
    }
}
