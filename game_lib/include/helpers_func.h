#pragma once

#include <iostream>

#include "game_logic.h"

void LogReport(ReportAction report, std::string message = {}, std::ostream &out = std::cout);

std::string TypeCellStr(TypeCell cell);

int get_rand(int start, int stop);