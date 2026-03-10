#pragma once
#include <string>
#include <vector>
#include "Player.h"

class Excavation
{
public:
    virtual ~Excavation() = default;

    virtual std::string GetName() const = 0;
    virtual int EnergyCost() const = 0;
    virtual std::vector<std::string> PossibleLoot() const = 0;

    virtual void Explore(Player& player) = 0;
};

class DesertExcavation : public Excavation
{
public:
    std::string GetName() const override;
    int EnergyCost() const override;
    std::vector<std::string> PossibleLoot() const override;
    void Explore(Player& player) override;
};

class ForestExcavation : public Excavation
{
public:
    std::string GetName() const override;
    int EnergyCost() const override;
    std::vector<std::string> PossibleLoot() const override;
    void Explore(Player& player) override;
};

class MountainExcavation : public Excavation
{
public:
    std::string GetName() const override;
    int EnergyCost() const override;
    std::vector<std::string> PossibleLoot() const override;
    void Explore(Player& player) override;
};