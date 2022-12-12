#pragma once

#include "game_logic.h"


/*
    1. Index out of range
    2. Cell is not empty
    3. The game is already over
    4. The player is not registered in the game room
    5. The other player must move
    6. Rollback is only available with a bot
    7. Bot can't rollback
    8. Rollback is impossible
    9. Your move should have been in the 7th field 3x3
*/

inline extern const ReportAction reportCode1 = {
        .isValid = false,
        .error = {
                .codeError = 1,
                .messageError = "Index out of range",
        }
};

inline extern const ReportAction reportCode2 = {
        .isValid = false,
        .error = {
                .codeError = 2,
                .messageError = "Cell is not empty",
        }
};

inline extern const ReportAction reportCode3 = {
        .isValid = false,
        .error = {
                .codeError = 3,
                .messageError = "The game is already over",
        }
};

inline extern const ReportAction reportCode4 = {
        .isValid = false,
        .error = {
                .codeError = 4,
                .messageError = "The player is not registered in the game room",
        }
};

inline extern const ReportAction reportCode5 = {
        .isValid = false,
        .error = {
                .codeError = 5,
                .messageError = "The other player must move",
        }
};

inline extern const ReportAction reportCode6 = {
        .isValid = false,
        .error = {
                .codeError = 5,
                .messageError = "Rollback is only available with a bot",
        }
};

inline extern const ReportAction reportCode7 = {
        .isValid = false,
        .error = {
                .codeError = 7,
                .messageError = "Bot can't rollback",
        }
};

inline extern const ReportAction reportCode8 = {
        .isValid = false,
        .error = {
                .codeError = 8,
                .messageError = "Rollback is impossible",
        }
};

inline extern const ReportAction reportCode9 = {
        .isValid = false,
        .error = {
                .codeError = 9,
                .messageError = "By the rules you can't play on this field 3x3",
        }
};
