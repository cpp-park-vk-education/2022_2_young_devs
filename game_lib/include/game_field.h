#pragma once

#include <vector>
#include <mutex>

#include "game_logic.h"



template <class T>
class BaseGameField
{
private:
public:
    virtual size_t  Size() const = 0;
    virtual void    Clear() = 0;    
    virtual T       At(size_t index) const = 0;
    virtual void    Set(size_t index, T value) = 0;
    virtual void    Rollback(size_t countSteps, std::vector<StepInfo> &steps) = 0;

};

class T_GameField : public BaseGameField<TypeCell>
{
protected:
public:
    virtual size_t   Dimension() = 0;
    virtual size_t   Size() const = 0;
    virtual void     Clear() = 0;
    virtual TypeCell At(size_t index) const = 0;
    virtual void     Set(size_t index, TypeCell value) = 0;
    virtual void     Rollback(size_t countSteps, std::vector<StepInfo> &steps) = 0;
    virtual GameResult IsEnd() = 0;
};

class ST_Field;

class OT_Field : public T_GameField
{
private:
    mutable std::recursive_mutex      _mutex;
    size_t                  _dim;
    std::vector<TypeCell>   _field;
    std::vector<int>        _sums;
    friend ST_Field;
public:
    OT_Field(size_t dim = 3);
    // если убрать = delete, компиляция не падает
    OT_Field(const OT_Field &) = delete;
    OT_Field &operator=(const OT_Field &) = delete;
    OT_Field(OT_Field &&) = delete;
    virtual size_t   Dimension() override;
    virtual size_t   Size() const override;
    virtual void     Clear() override;
    virtual TypeCell At(size_t index) const override;
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
    mutable std::recursive_mutex    _mutex;
    size_t                  _dim_fields;    // 3
    size_t                  _dim;           // 9
    std::array<OT_Field, 9> _field;
    OT_Field                _miniature;
public:
    ST_Field();
    virtual size_t   Dimension() override;
    virtual size_t   Size() const override;
    virtual void     Clear() override;
    virtual TypeCell At(size_t index) const override;
    virtual void     Set(size_t index, TypeCell cell) override;
    virtual void     Rollback(size_t countSteps, std::vector<StepInfo> &steps) override;
    virtual GameResult  IsEnd() override;
private:
    void CommitStep(size_t index, TypeCell cell);
};