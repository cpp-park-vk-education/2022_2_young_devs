#pragma once

#include <vector>

#include "game_logic.h"



template <class T>
class GameField
{
private:
public:
    virtual size_t  Size() = 0;
    virtual void    Clear() = 0;    
    virtual T       At(size_t index) = 0;
    virtual void    Set(size_t index, T value) = 0;
    virtual void    Rollback(size_t countSteps, std::vector<StepInfo> &steps) = 0;

};

class T_GameField : public GameField<TypeCell>
{
protected:
public:
    virtual size_t   Dimension() = 0;
    virtual size_t   Size() = 0;
    virtual void     Clear() = 0;
    virtual TypeCell At(size_t index) = 0;
    virtual void     Set(size_t index, TypeCell value) = 0;
    virtual void     Rollback(size_t countSteps, std::vector<StepInfo> &steps) = 0;
    virtual GameResult IsEnd() = 0;
};

class ST_Field;

class OT_Field : public T_GameField
{
private:
    size_t                  _dim;
    std::vector<TypeCell>   _field;
    std::vector<int>        _sums;
    friend ST_Field;
public:
    OT_Field(size_t dim);
    virtual size_t   Dimension() override;
    virtual size_t   Size() override;
    virtual void     Clear() override;
    virtual TypeCell At(size_t index) override;
    virtual void     Set(size_t index, TypeCell cell) override;
    virtual void     Rollback(size_t countSteps, std::vector<StepInfo> &steps) override;
    virtual GameResult  IsEnd() override;
private:
    void CommitStep(size_t index, TypeCell cell);
    void RecalcSums();
};

class ST_Field : public T_GameField
{
private:
    size_t                  _dim_fields;    // 3
    size_t                  _dim;           // 9
    std::vector<OT_Field>   _field;
    OT_Field                _miniature;
public:
    ST_Field();
    virtual size_t   Dimension() override;
    virtual size_t   Size() override;
    virtual void     Clear() override;
    virtual TypeCell At(size_t index) override;
    virtual void     Set(size_t index, TypeCell cell) override;
    virtual void     Rollback(size_t countSteps, std::vector<StepInfo> &steps) override;
    virtual GameResult  IsEnd() override;
private:
    void CommitStep(size_t index, TypeCell cell);
};