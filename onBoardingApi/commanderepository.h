#ifndef COMMANDEREPOSITORY_H
#define COMMANDEREPOSITORY_H

#pragma once
#include <QList>
#include <QString>
#include <optional>

struct Commande
{
    std::optional<int> uid = std::nullopt;
    std::optional<QString> title = std::nullopt;
    std::optional<int> serialNumber = std::nullopt;
    int currentStep = 0;
    bool currentlyGoingOn = false;
    bool loaded = false;
};

struct CommandeStep0
{
    int reference = 0;
    int reportNumber = 0;
    QString customer;
    QString customerSerialNumber;
    QString serialNumberAlorem;
    QString type;
    int commandeUid = 0;
};

struct CommandeStep1
{
    int reference = 0;
    std::optional<bool> fabricationPlateAdr;
    std::optional<bool> aloremPlate;
    std::optional<bool> bookletPouch;
    std::optional<bool> userManual;
    std::optional<bool> instructionOfUse;
    std::optional<bool> certificatesADR;
    std::optional<bool> groundingAdhesive;
    std::optional<bool> conformityCertificateMarking;
    int commandeUid = 0;
};

struct CommandeStep2
{
    int reference = 0;
    QString marquePrincipale;
    int tare = 0;
    QString material;
    int capacity20 = 0;
    int grossMass = 0;
    QString fabricationDate;
    int shellThickness = 0;
    std::optional<bool> pictogramStacking;
    int weightStacking = 0;
    int commandeUid = 0;
};

struct CommandeStep3
{
    int reference = 0;
    std::optional<int> bottomRetentionFace;
    std::optional<int> bottomRetentionRight;
    std::optional<int> bottomRetentionLeft;
    std::optional<int> bottomRetentionBehind;
    std::optional<int> upperRetention;
    std::optional<int> liftingRings;
    std::optional<int> forkliftPass;
    std::optional<int> dashboard;
    bool unauthorizedRepair = false;
    int commandeUid = 0;
};

struct CommandeStep4
{
    int reference = 0;
    bool internalNA = false;
    std::optional<bool> internalOK;
    bool internalClean = false;
    bool internalObjectInside = false;
    bool internalPollution = false;
    int commandeUid = 0;
};

struct CommandeStep5
{
    int reference = 0;

    bool epaisseurNA = false;

    int epaisseurMinSideFront = 0;
    int epaisseurMinSideBack = 0;
    int epaisseurMinSideRight = 0;
    int epaisseurMinSideLeft = 0;

    int epaisseurSideFrontResult1 = 0;
    int epaisseurSideFrontResult2 = 0;
    int epaisseurSideFrontResult3 = 0;
    int epaisseurSideFrontResult4 = 0;
    int epaisseurSideFrontResult5 = 0;

    int epaisseurSideBackResult1 = 0;
    int epaisseurSideBackResult2 = 0;
    int epaisseurSideBackResult3 = 0;
    int epaisseurSideBackResult4 = 0;
    int epaisseurSideBackResult5 = 0;

    int epaisseurSideRightResult1 = 0;
    int epaisseurSideRightResult2 = 0;
    int epaisseurSideRightResult3 = 0;
    int epaisseurSideRightResult4 = 0;
    int epaisseurSideRightResult5 = 0;

    int epaisseurSideLeftResult1 = 0;
    int epaisseurSideLeftResult2 = 0;
    int epaisseurSideLeftResult3 = 0;
    int epaisseurSideLeftResult4 = 0;
    int epaisseurSideLeftResult5 = 0;

    int commandeUid = 0;
};


struct CommandeStep6
{
    int reference = 0;

    std::optional<bool> etancheiteOK1;
    QString etancheiteDate1;
    float etancheiteBar1 = 0.0f;

    std::optional<bool> etancheiteOK2;
    QString etancheiteDate2;
    float etancheiteBar2 = 0.0f;

    int commandeUid = 0;
};


struct CommandeComplete
{
    Commande commande;

    std::optional<CommandeStep0> step0;
    std::optional<CommandeStep1> step1;
    std::optional<CommandeStep2> step2;
    std::optional<CommandeStep3> step3;
    std::optional<CommandeStep4> step4;
    std::optional<CommandeStep5> step5;
    std::optional<CommandeStep6> step6;
};


enum class UpdateCommandeResult
{
    Success,
    NotFound,
    Error
};

class CommandeRepository
{
public:
    QList<Commande> findAll();
    std::optional<CommandeComplete> findById(int uid);
    std::optional<int> createCommande(const CommandeComplete &commande);
    UpdateCommandeResult updateCommande(int uid, const CommandeComplete &commande);
};

#endif // COMMANDEREPOSITORY_H
