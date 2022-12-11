#include "helpers_func.h"


constexpr size_t n = 6;
constexpr char sym = ' ';

std::string TypeActionStr(TypeAction action)
{
    std::string strAction;
#define PRINT(a) strAction = #a;
    switch (action)
    {
        case TypeAction::Step: {
            PRINT(Step);
            break;
        }
        case TypeAction::Rollback: {
            PRINT(Rollback);
            break;
        }
        default: {
            strAction = "unknown";
        }
    }
    return strAction;
}

std::string TypeStatusStr(TypeStatus status)
{
    std::string strStatus;
#define PRINT(a) strStatus = #a;
    switch (status)
    {
        case TypeStatus::Active: {
            PRINT(Active);
            break;
        }
        case TypeStatus::Stopped: {
            PRINT(Stopped);
            break;
        }
        case TypeStatus::Finished: {
            PRINT(Finished);
            break;
        }
        default: {
            strStatus = "unknown";
        }
    }
    return strStatus;
}

void LogPlayer(Player player, std::ostream &out = std::cout)
{
    out << std::string(n, sym ) << "id = " <<   player.id << "\n";
    out << std::string(n, sym ) << "cell = " << (player.cell == TypeCell::X ? "X" : "O") << "\n";
    out << std::string(n, sym ) << "isBot? " << player.isBot << "\n";  
}

void LogReport(ReportAction report, std::ostream &out)
{
    
    out << std::string(20, '*' ) << "\n";
    out << "> Room\n";
    out << std::string(n, sym ) << "room_id = " << report.room_id << "\n";
    out << "> Players\n";
    
    LogPlayer(report.player);
    
    out << "> DataAction\n";
    if (!report.data.data.empty())
    {
        out << std::string(n, sym ) << "data  = " << report.data.data << "\n";
    }
    out << std::string(n, sym ) << "value = " << report.data.value << "\n";

    out << "> TypeGame\n";
    out << std::string(n, sym ) << (report.typeGame == TypeGame::OT ? "OT" : "ST") << "\n";

    out << "> TypeAction\n";
    out << std::string(n, sym ) << TypeActionStr(report.typeAction) << "\n";

    out << "> TypeStatus\n";
    out << std::string(n, sym ) << TypeStatusStr(report.status) << "\n";

    out << "> Valid\n";
    out << std::string(n, sym ) << "is valid? " << report.isValid << "\n";
    if (!report.isValid)
    {
        out << std::string(n, sym ) << "codeError    = " << report.error.codeError << "\n";
        out << std::string(n, sym ) << "messageError = " << report.error.messageError << "\n";

    }

    out << "> GameResult\n";
    out << std::string(n, sym ) << "is end? " << report.result.isEnd << "\n";
    if (report.result.isEnd)
    {
        out << std::string(n, sym ) << "winner = " << (report.result.winnerCell == TypeCell::X ? "X" : "O") << "\n";
        out << "> Winner\n";
        LogPlayer(report.result.winner);
        out << std::string(n, sym ) << "draw? " << report.result.draw << "\n";
    }
    
    out << "> Steps\n";
    for (size_t i = 0; i < report.steps.size(); ++i)
    {
        out << std::string(n * 2, sym ) << "player_id = " << report.steps[i].player_id << "\n";
        out << std::string(n * 2, sym ) << "index     = " << report.steps[i].index << "\n";
        out << std::string(n * 2, sym ) << "cell      = " << (report.steps[i].cell == X ? "X" : "O") << "\n";
        out << std::string(n * 2, sym ) << std::string(n * 2, '-' ) << "\n";
    }

    out << std::string(20, '*' ) << "\n\n";
}