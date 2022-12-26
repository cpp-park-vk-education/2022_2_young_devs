#include <cassert>
#include <sstream>

#include "helpers_func.h"
#include "game_field.h"


constexpr size_t n = 6;
constexpr char sym = ' ';

using namespace std::string_literals;

const bool OUTPUT_WITH_ALL_STEPS = true;

std::string TypeCellStr(TypeCell cell)
{
    std::string strCell;
#define PRINT(a) strCell = #a;
    switch (cell)
    {
        case X: {
            PRINT(X);
            break;
        }
        case O: {
            PRINT(O);
            break;
        }
        case E: {
            strCell = "-";
            break;
        }
        default: {
            strCell = "~";
        }
    }
    return strCell;
}

int TypeCellNum(TypeCell cell)
{
    int numCell = 0;
    switch (cell)
    {
        case X: {
            numCell = 1;
            break;
        }
        case O: {
            numCell = -1;
            break;
        }
        case E: {
            numCell = 0;
            break;
        }
        default: {
            numCell = 4;
        }
    }
    return numCell;
}

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

void LogReport(ReportAction report, std::string message, std::ostream &out)
{
    if (!message.empty())
    {
        out << message << "\n";
    }
    out << std::string(20, '*' ) << "\n";
    out << "> Room\n";
    out << std::string(n, sym ) << "room_id = " << report.room_id << "\n";
    out << "> Players\n";
    
    LogPlayer(report.player, out);
    
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
        LogPlayer(report.result.winner, out);
        out << std::string(n, sym ) << "draw? " << report.result.draw << "\n";
    }
    
    out << "> Steps\n";
    out << std::string(n, sym ) << "steps_count = " << report.steps.size() << "\n";


    // Для стресс-тестов, слишком большой вывод...
    if (OUTPUT_WITH_ALL_STEPS)
    {
        for (size_t i = 0; i < report.steps.size(); ++i)
        {
            out << std::string(n * 2, sym ) << "player_id = " << report.steps[i].player_id << "\n";
            out << std::string(n * 2, sym ) << "index     = " << report.steps[i].index << "\n";
            out << std::string(n * 2, sym ) << "cell      = " << (report.steps[i].cell == X ? "X" : "O") << "\n";
            out << std::string(n * 2, sym ) << std::string(n * 2, '-' ) << "\n";
        }

        out << std::string(20, '*' ) << "\n\n";
    }
}

std::string strJson(ReportAction report, bool save)
{
    std::stringstream json;
    json << "{" << "\"room_id\":" << report.room_id\
        << "," << "\"player_id\":" << report.player.id\
        << "," << "\"player_cell\":" << TypeCellNum(report.player.cell)\
        << "," << "\"is_valid\":" << report.isValid\
        // если id_valid = false
        << "," << "\"message_error\":" << "\""s << report.error.messageError << "\""s\
        << "," << "\"code_error\":"<< report.error.codeError\
        << "," << "\"is_end\":" << report.result.isEnd\
        // если is_end = true
        << "," << "\"draw\":" << report.result.draw\
        << "," << "\"winner\":" << report.result.winner.id\
        << "," << "\"winner_cell\":"  << TypeCellNum(report.result.winner.cell)\
        << "," << "\"field\":[";
        for (size_t i = 0; i < report.field->Size(); ++i)
        {
            if (i == report.field->Size() - 1)
            {
                json << TypeCellNum(report.field->At(i)) << "]";
            }
            else
            {
                json << TypeCellNum(report.field->At(i))  << ",";
            }
        }
        json << "," << "\"steps\":[";
        for (size_t i = 0; i < report.steps.size(); ++i)
        {
            json << "{"\
            << "\"player_id\":" <<  report.steps[i].player_id << ","\
            << "\"index\":" <<  report.steps[i].index << ","\
            << "\"cell\":" << TypeCellNum(report.steps[i].cell) << "}";

            if (i == report.steps.size() - 1)
            {
                json << "]";
            }
            else
            {
                json << ",";
            }
        }
        if (save)
        {
            json << ", \"save\" : true";
        }
        json << "}";
        return json.str();
}


// mt19937 ?
int get_rand(int start, int stop)
{
    assert(stop >= start);
    return rand() % (stop - start) + start;
}