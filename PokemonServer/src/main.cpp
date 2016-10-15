﻿#include <QCoreApplication>

#include "pokemon.h"
#include "pokemonfactory.h"

#include "catchunittest.h"
#include "./lib/sqlite3.h"

void PrintPokeData(Pokemon *pokemon) {
    cout << "Kind:" << kindOfString[pokemon->getKind()] <<  "\tName:" << pokemon->getName() << "\tAlive:" << aliveOfString[pokemon->getAlive()] <<
            "\tNature:" << natureOfString[pokemon->getNature()] << "\nCharacter:" << characterOfString[pokemon->getCharacter()] <<
            "\tLevel:" << pokemon->getLevel() << "\tExperiencePoint:"  << pokemon->getExperiencePoint() <<
            "\tAttackPoint:" << pokemon->getAttackPoint() << "\nDefencePoint:" << pokemon->getDefencePoint() <<
            "\tHP(C/T):" << pokemon->getCurrentHP() << "/" << pokemon->getTotalHP() <<
            "\tIntervalIncrease:" << pokemon->getIntervalIncrease() << "\tCriticalPoint:" << pokemon->getCriticalPoint() <<
            "\nState:" << stateOfString[pokemon->getState()] <<
            "\tSickPoint:" << pokemon->getSickPoint() << "\tSickCounter:" << pokemon->getSickCounter();

    int counterSetSize = pokemon->getCounterSet().size();
    cout << "\tCounterSet:";
    set<Nature>::iterator it = pokemon->getCounterSet().begin();
    for (int i = 0; i< counterSetSize; i++) {
        cout << natureOfString[*it];
        if (i < counterSetSize- 1)
            cout << "-";
        it++;
    }
    cout << endl;
    cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - " << endl;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    PokemonFactory *pokemonFactory = new PokemonFactory();
    cout << "Please name your pokemon: ";
    string name;
    cin >> name;
    Pokemon *charamander = pokemonFactory->CreatePokemon(CHARMELEON, 6, name);
    cout << "Please name your pokemon: ";
    cin >> name;
    Pokemon *pikachu = pokemonFactory->CreatePokemon(RAICHU, 15, name);

    PrintPokeData(charamander);
    PrintPokeData(pikachu);
    pikachu->Attack(charamander);
    charamander->DeadJudge();
    PrintPokeData(charamander);

    charamander->SpecialAttack(pikachu);
    charamander->DeadJudge();
    PrintPokeData(pikachu);

    delete pikachu;
    delete charamander;
    delete pokemonFactory;

    int result = Catch::Session().run( argc, argv );

    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;

    rc = sqlite3_open("test.db", &db);

    if( rc ){
       fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
       exit(0);
    }
    else{
       fprintf(stderr, "Opened database successfully\n");
    }
    sqlite3_close(db);

    return a.exec();
}
