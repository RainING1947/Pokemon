﻿#define _CRT_RAND_S
#include "pokemon.h"

using namespace std;
/* srand+rand 精度保持在1秒 1秒内生成的随机数相同
 * rand_s 1秒内可以产生不同随机数*/
unsigned int Random(int max) {
    errno_t err;
        unsigned int number;
        err = rand_s(&number);
        if(err != 0)
        {
          return 0;
        }
        return (unsigned int)((double)number / ((double)UINT_MAX + 1) * double(max)) + 1;
}

struct PokemonInfo Pokemon::ToPokeStruInfo()
{
    struct PokemonInfo pi = {
        this->_owner_, this->_name_, (int)this->_nature_, (int)this->_kind_,
                (int)this->_character_, this->_level_, this->_experiencePoint_,
                this->_attackPoint_, this->_defencePoint_, this->_totalHP_,
                this->_currentHP_, this->_intervalIncrease_, this->_criticalPoint_,
                (int)this->_state_, this->_sickCounter_, this->_sickPoint_, (int)this->_alive_
    };
    return pi;
}

/* 攻击函数
 * 暴击双倍伤害 依小概率造成烧伤，冻伤，中毒和麻痹效果 火属性不能对火属性造成烧伤 其他属性类比*/
void Pokemon::Attack(Pokemon *dePokemon) {
    int attackDamage = 0;
    //ciritical strike : double attack point
    if (this->CriticalStrike()) {
        attackDamage = (2* this->getAttackPoint())- dePokemon->getDefencePoint();
    }
    else {
        attackDamage = this->getAttackPoint()- dePokemon->getDefencePoint();
    }
    if (attackDamage < 0) {
        attackDamage = 0;
    }
    dePokemon->setCurrentHP(dePokemon->getCurrentHP()- attackDamage);
   }

void Pokemon::SpecialAttackDamage(Pokemon *dePokemon) {
    int specialDamage = this->getAttackPoint() - dePokemon->getDefencePoint();
    bool counter = false;
    //find whether pokemon being attacked is countered by
    for (auto c : this->getCounterVec())
        if (c == dePokemon->getNature())
            counter = true;

    //find whether this pokemon is be countered by another pokemon
    bool beCounter = false;
    for (auto c : dePokemon->getCounterVec())
        if (c == this->getNature())
            beCounter = true;

    //cause double damage if counter
    if (counter) {
        specialDamage *= (int)2* specialDamage;
    }
    //cause half damage if being countered
    else if (beCounter){
        specialDamage = (int)0.5* specialDamage;
    }
    if (specialDamage < 0)
        specialDamage = 0;
    dePokemon->setCurrentHP(dePokemon->getCurrentHP() - specialDamage);
}

//possibility generator
int randFunction() {
    const int MIN = 0;
    const int MAX = 99;
    return MIN + Random(MAX - MIN);
}

//ensick possibility is constant 20%
bool Pokemon::EnSickPossible() {
    int randNum = randFunction();
    if (randNum <= 19) {
        cout << "EnSick True" << endl;
        cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - " << endl;
        return true;
    }
    cout << "EnSick False" << endl;
    cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - " << endl;
    return false;
}

//critical possibility depends on pokemon's attribute
bool Pokemon::CriticalStrike() {
    int randNum = randFunction();
    if (randNum <= this->getCriticalPoint()) {
        cout << "Critical True" << endl;
        cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - " << endl;
        return true;
    }
    cout << "Critical False" << endl;
    cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - " << endl;
    return false;
}

//持续伤害函数 不健康状态下每回合造成伤害 伤害大小取决于造成持续伤害时攻击方的攻击力
void Pokemon::Hurt()
{
    if (this->getState() != HEALTHY) {
        this->setCurrentHP(this->getCurrentHP() - this->getSickPoint());
        this->setSickCounter(this->getSickCounter()- 1);
    }
}

void Pokemon::DeadJudge()
{
    if (this->getCurrentHP() <= 0) {
        this->setAlive(DEAD);
        this->setCurrentHP(0);
    }
}

/* Upgrade does not manage experience-increase after battle
 * Gain experience and judge whether it upgrades or not and decide
 * call Upgrade() or not
 */
void Pokemon::Upgrade()
{
    //等级不能超过15级 战斗模块中战斗后获得经验函数判断是否15级进入Upgrade函数后可取消以下判定
    if (this->getLevel() == 15)
        return;
    this->setLevel(this->getLevel()+ 1);
    //cout << "Kind: " << kindOfString[this->getKind()] << "  Level:" << this->getLevel() << "  Kind:" << kindOfString[this->getKind()] << endl;
    switch (this->getCharacter()) {
    case AGGRESIVE:
        this->setAttackPoint(this->getAttackPoint() + UpGradeAttrIncStrong);
        this->setDefencePoint(this->getDefencePoint() + UpGradeAttrIncNormal);
        this->setTotalHP(this->getTotalHP() + UpGradeAttrIncNormal * 2);
        this->setIntervalIncrease(this->getIntervalIncrease() + UpGradeAttrIncWeak);
        break;
    case DEFENSIVE:
        this->setAttackPoint(this->getAttackPoint() + UpGradeAttrIncNormal);
        this->setDefencePoint(this->getDefencePoint() + UpGradeAttrIncStrong);
        this->setTotalHP(this->getTotalHP() + UpGradeAttrIncNormal * 2);
        this->setIntervalIncrease(this->getIntervalIncrease() + UpGradeAttrIncWeak);
        break;
    case TANK:
        this->setAttackPoint(this->getAttackPoint() + UpGradeAttrIncNormal);
        this->setDefencePoint(this->getDefencePoint() + UpGradeAttrIncNormal);
        this->setTotalHP(this->getTotalHP() + UpGradeAttrIncStrong * 2);
        this->setIntervalIncrease(this->getIntervalIncrease() + UpGradeAttrIncWeak);
        break;
    case QUICK:
        this->setAttackPoint(this->getAttackPoint() + UpGradeAttrIncNormal);
        this->setDefencePoint(this->getDefencePoint() + UpGradeAttrIncNormal);
        this->setTotalHP(this->getTotalHP() + UpGradeAttrIncNormal * 2);
        this->setIntervalIncrease(this->getIntervalIncrease() + UpGradeAttrIncNormal);
        break;
    default:
        cout << "只包含 攻击 防御 肉盾 敏捷 四种性格 如有改动请修改case语句" << endl;
    };
    this->setCriticalPoint(this->getCriticalPoint() + (this->getLevel()% 3== 0? 5: 0));
    if (this->getLevel() == EL1 && !(Helper::Helper::isInStandard(this->getKind(), IntermediatePokemon)) && !(Helper::isInStandard(this->getKind(), LastPokemon))) {
        this->Evolution(EL1);
    }
    if (this->getLevel() == EL2 && !(Helper::Helper::isInStandard(this->getKind(), LastPokemon))) {
        this->Evolution(EL2);
    }
}

void Pokemon::Evolution(int evoLevel) {
    this->setKind((Kind)(this->getKind()+ 1));
    switch (evoLevel) {
        //evolute from stage 1 to stage 2, attributes improve a little
        case EL1:
            this->setAttackPoint((int) this->getAttackPoint()* 1.2);
            this->setDefencePoint((int) this->getDefencePoint()* 1.2);
            this->setTotalHP((int) this->getTotalHP()* 1.5);
            break;
        //evolute from stage 2 to stage 3, attributes improve a lot
        case EL2:
            this->setAttackPoint((int) this->getAttackPoint()* 1.5);
            this->setDefencePoint((int) this->getDefencePoint()* 1.5);
            this->setTotalHP((int) this->getTotalHP()* 1.8);
            break;
        default:
            cout << "Errro In Evolution Function: illegal evolution level" << endl;
            break;
    };
}

//attribute gift generator
int GiftGenFunc(int MIN, int MAX) {
    return MIN + Random(MAX - MIN);
}

Fire::Fire(Kind kind, int level, string name, string owner) {
    this->setOwner(owner);
    this->setNature(FIRE);
    this->setKind(kind);
    this->setName(name);
    this->setCharacter((Character)GiftGenFunc(0, 3));
    this->setLevel(1);
    this->setExperiencePoint(0);
    this->setAttackPoint(GiftGenFunc(20, 30));
    this->setDefencePoint(GiftGenFunc(10, 20));
    this->setTotalHP(GiftGenFunc(50, 60));
    this->setCurrentHP(this->getTotalHP());
    this->setIntervalIncrease(GiftGenFunc(2, 4));
    this->setCriticalPoint(10);
    vector<Nature> counterNature;
    counterNature.push_back(BUSH);
    this->setCounter(counterNature);
    this->setState(HEALTHY);
    this->setSickPoint(0);
    this->setSickCounter(0);
    this->setAlive(ALIVE);
    while (this->getLevel() < level) {
        this->Upgrade();
    }
    this->setExperiencePoint(ExpGrade[this->getLevel()]);
    this->setCurrentHP(this->getTotalHP());
}

Fire::Fire(PokemonInfo pokemonInfo)
{
    this->setOwner(pokemonInfo.owner);
    this->setNature(FIRE);
    this->setKind((Kind)pokemonInfo.kind);
    this->setName(pokemonInfo.name);
    this->setCharacter((Character)pokemonInfo.character);
    this->setLevel(pokemonInfo.level);
    this->setExperiencePoint(pokemonInfo.experiencePoint);
    this->setAttackPoint(pokemonInfo.attackPoint);
    this->setDefencePoint(pokemonInfo.defencePoint);
    this->setTotalHP(pokemonInfo.totalHP);
    this->setCurrentHP(pokemonInfo.currentHP);
    this->setIntervalIncrease(pokemonInfo.intervalIncrease);
    this->setCriticalPoint(pokemonInfo.criticalPoint);
    vector<Nature> counterNature;
    counterNature.push_back(BUSH);
    this->setCounter(counterNature);
    this->setState((State)pokemonInfo.state);
    this->setSickPoint(pokemonInfo.sickPoint);
    this->setSickCounter(pokemonInfo.sickCounter);
    this->setAlive(pokemonInfo.alive);
}

void Fire::SpecialAttack(Pokemon *dePokemon) {
    this->SpecialAttackDamage(dePokemon);
    if (EnSickPossible()) {
        this->EnSick(dePokemon);
    }
}

void Fire::EnSick(Pokemon *sickPokemon) {
    sickPokemon->setState(BURNED);
    //sick damage is constantly quarter
    sickPokemon->setSickPoint((int)(0.25* this->getAttackPoint()));
    //the number of round that pokemon is sick is 3
    sickPokemon->setSickCounter(3);
}

Water::Water(Kind kind, int level, string name, string owner) {
    this->setOwner(owner);
    this->setNature(WATER);
    this->setKind(kind);
    this->setName(name);
    this->setCharacter((Character)GiftGenFunc(0, 3));
    this->setLevel(1);
    this->setExperiencePoint(0);
    this->setAttackPoint(GiftGenFunc(20, 30));
    this->setDefencePoint(GiftGenFunc(10, 20));
    this->setTotalHP(GiftGenFunc(50, 60));
    this->setCurrentHP(this->getTotalHP());
    this->setIntervalIncrease(GiftGenFunc(2, 4));
    this->setCriticalPoint(10);
    vector<Nature> counterNature;
    counterNature.push_back(FIRE);
    this->setCounter(counterNature);
    this->setState(HEALTHY);
    this->setSickPoint(0);
    this->setSickCounter(0);
    this->setAlive(ALIVE);
    while (this->getLevel() < level) {
        this->Upgrade();
    }
    this->setExperiencePoint(ExpGrade[this->getLevel()]);
    this->setCurrentHP(this->getTotalHP());
}

Water::Water(PokemonInfo pokemonInfo)
{
    this->setOwner(pokemonInfo.owner);
    this->setNature(WATER);
    this->setKind((Kind)pokemonInfo.kind);
    this->setName(pokemonInfo.name);
    this->setCharacter((Character)pokemonInfo.character);
    this->setLevel(pokemonInfo.level);
    this->setExperiencePoint(pokemonInfo.experiencePoint);
    this->setAttackPoint(pokemonInfo.attackPoint);
    this->setDefencePoint(pokemonInfo.defencePoint);
    this->setTotalHP(pokemonInfo.totalHP);
    this->setCurrentHP(pokemonInfo.currentHP);
    this->setIntervalIncrease(pokemonInfo.intervalIncrease);
    this->setCriticalPoint(pokemonInfo.criticalPoint);
    vector<Nature> counterNature;
    counterNature.push_back(FIRE);
    this->setCounter(counterNature);
    this->setState((State)pokemonInfo.state);
    this->setSickPoint(pokemonInfo.sickPoint);
    this->setSickCounter(pokemonInfo.sickCounter);
    this->setAlive(pokemonInfo.alive);
}

void Water::SpecialAttack(Pokemon *dePokemon) {
    this->SpecialAttackDamage(dePokemon);
    if (EnSickPossible()) {
        this->EnSick(dePokemon);
    }
}

void Water::EnSick(Pokemon *sickPokemon) {
    sickPokemon->setState(DROWNED);
    //sick damage is constantly quarter
    sickPokemon->setSickPoint((int) 0.2* this->getAttackPoint());
    //the number of round that pokemon is sick is 3
    sickPokemon->setSickCounter(3);
}

Bush::Bush(Kind kind, int level, string name, string owner) {
    this->setOwner(owner);
    this->setNature(BUSH);
    this->setKind(kind);
    this->setName(name);
    this->setCharacter((Character)GiftGenFunc(0, 3));
    this->setLevel(1);
    this->setExperiencePoint(0);
    this->setAttackPoint(GiftGenFunc(20, 30));
    this->setDefencePoint(GiftGenFunc(10, 20));
    this->setTotalHP(GiftGenFunc(50, 60));
    this->setCurrentHP(this->getTotalHP());
    this->setIntervalIncrease(GiftGenFunc(2, 4));
    this->setCriticalPoint(10);
    vector<Nature> counterNature;
    counterNature.push_back(WATER);
    counterNature.push_back(ELECTRICITY);
    this->setCounter(counterNature);
    this->setState(HEALTHY);
    this->setSickPoint(0);
    this->setSickCounter(0);
    this->setAlive(ALIVE);
    while (this->getLevel() < level) {
        this->Upgrade();
    }
    this->setExperiencePoint(ExpGrade[this->getLevel()]);
    this->setCurrentHP(this->getTotalHP());
}

Bush::Bush(PokemonInfo pokemonInfo)
{
    this->setOwner(pokemonInfo.owner);
    this->setNature(BUSH);
    this->setKind((Kind)pokemonInfo.kind);
    this->setName(pokemonInfo.name);
    this->setCharacter((Character)pokemonInfo.character);
    this->setLevel(pokemonInfo.level);
    this->setExperiencePoint(pokemonInfo.experiencePoint);
    this->setAttackPoint(pokemonInfo.attackPoint);
    this->setDefencePoint(pokemonInfo.defencePoint);
    this->setTotalHP(pokemonInfo.totalHP);
    this->setCurrentHP(pokemonInfo.currentHP);
    this->setIntervalIncrease(pokemonInfo.intervalIncrease);
    this->setCriticalPoint(pokemonInfo.criticalPoint);
    vector<Nature> counterNature;
    counterNature.push_back(WATER);
    counterNature.push_back(ELECTRICITY);
    this->setCounter(counterNature);
    this->setState((State)pokemonInfo.state);
    this->setSickPoint(pokemonInfo.sickPoint);
    this->setSickCounter(pokemonInfo.sickCounter);
    this->setAlive(pokemonInfo.alive);
}

void Bush::SpecialAttack(Pokemon *dePokemon) {
    this->SpecialAttackDamage(dePokemon);
    if (EnSickPossible()) {
        this->EnSick(dePokemon);
    }
}

void Bush::EnSick(Pokemon *sickPokemon) {
    sickPokemon->setState(POISONED);
    //sick damage is constantly quarter
    sickPokemon->setSickPoint((int) 0.2* this->getAttackPoint());
    //the number of round that pokemon is sick is 3
    sickPokemon->setSickCounter(3);
}

Electricity::Electricity(Kind kind, int level, string name, string owner) {
    this->setOwner(owner);
    this->setNature(ELECTRICITY);
    this->setKind(kind);
    this->setName(name);
    this->setCharacter((Character)GiftGenFunc(0, 3));
    this->setLevel(1);
    this->setExperiencePoint(0);
    this->setAttackPoint(GiftGenFunc(20, 30));
    this->setDefencePoint(GiftGenFunc(10, 20));
    this->setTotalHP(GiftGenFunc(50, 60));
    this->setCurrentHP(this->getTotalHP());
    this->setIntervalIncrease(GiftGenFunc(2, 4));
    this->setCriticalPoint(10);
    vector<Nature> counterNature;
    counterNature.push_back(WATER);
    this->setCounter(counterNature);
    this->setState(HEALTHY);
    this->setSickPoint(0);
    this->setSickCounter(0);
    this->setAlive(ALIVE);
    while (this->getLevel() < level) {
        this->Upgrade();
    }
    this->setExperiencePoint(ExpGrade[this->getLevel()]);
    this->setCurrentHP(this->getTotalHP());
}

Electricity::Electricity(PokemonInfo pokemonInfo)
{
    this->setOwner(pokemonInfo.owner);
    this->setNature(ELECTRICITY);
    this->setKind((Kind)pokemonInfo.kind);
    this->setName(pokemonInfo.name);
    this->setCharacter((Character)pokemonInfo.character);
    this->setLevel(pokemonInfo.level);
    this->setExperiencePoint(pokemonInfo.experiencePoint);
    this->setAttackPoint(pokemonInfo.attackPoint);
    this->setDefencePoint(pokemonInfo.defencePoint);
    this->setTotalHP(pokemonInfo.totalHP);
    this->setCurrentHP(pokemonInfo.currentHP);
    this->setIntervalIncrease(pokemonInfo.intervalIncrease);
    this->setCriticalPoint(pokemonInfo.criticalPoint);
    vector<Nature> counterNature;
    counterNature.push_back(WATER);
    this->setCounter(counterNature);
    this->setState((State)pokemonInfo.state);
    this->setSickPoint(pokemonInfo.sickPoint);
    this->setSickCounter(pokemonInfo.sickCounter);
    this->setAlive(pokemonInfo.alive);
}

void Electricity::SpecialAttack(Pokemon *dePokemon) {
    this->SpecialAttackDamage(dePokemon);
    if (EnSickPossible()) {
        this->EnSick(dePokemon);
    }
}

void Electricity::EnSick(Pokemon *sickPokemon) {
    sickPokemon->setState(FROZEN);
    //sick damage is constantly quarter
    sickPokemon->setSickPoint((int) 0.2* this->getAttackPoint());
    //the number of round that pokemon is sick is 3
    sickPokemon->setSickCounter(3);
}

