#pragma once
#include <iostream>
#include <string>
#include <vector>

 class DataBase
 {
 private:
     virtual void insert(const std::string& table, const std::vector<std::string>& values) = 0;
     virtual std::vector<std::string> select(const std::string& table, const std::vector<std::string>& what = std::vector<std::string>(), const std::string& param = "") = 0;
     virtual void update(const std::string& table, const std::vector<std::string>& set, const std::string& where = "") = 0;
     virtual void delete_(const std::string& table, const std::string& where = "") = 0;
     virtual void create(const std::string& table, const std::vector<std::string>& arg) = 0;
     virtual void drop(const std::string& table, const std::vector<std::string>& arg) = 0;
 };
