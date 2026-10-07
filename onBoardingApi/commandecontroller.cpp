#include "CommandeController.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

CommandeController::CommandeController(QHttpServer &server,
                                       CommandeService &service)
    : service(service)
{
    registerRoutes(server);
}

std::optional<CommandeComplete> parseCommandeComplete(
    const QJsonObject &json);

std::optional<CommandeComplete>
CommandeController::parseCommandeComplete(const QJsonObject &json)
{
    CommandeComplete complete;

    // -------------------------
    // Commande
    // -------------------------

    if (json.contains("uid") && !json["uid"].isNull())
        complete.commande.uid = json["uid"].toInt();

    if (json.contains("title") && !json["title"].isNull())
        complete.commande.title = json["title"].toString();

    if (json.contains("serialNumber") && !json["serialNumber"].isNull())
        complete.commande.serialNumber = json["serialNumber"].toInt();

    if (json.contains("currentStep") && !json["currentStep"].isNull())
        complete.commande.currentStep = json["currentStep"].toInt();

    if (json.contains("currentlyGoingOn") && !json["currentlyGoingOn"].isNull())
        complete.commande.currentlyGoingOn = json["currentlyGoingOn"].toBool();

    if (json.contains("loaded") && !json["loaded"].isNull())
        complete.commande.loaded = json["loaded"].toBool();

    // -------------------------
    // Steps
    // -------------------------

    if (json.contains("steps") && json["steps"].isArray())
    {
        const QJsonArray steps = json["steps"].toArray();

        for (const QJsonValue &value : steps)
        {
            if (!value.isObject())
                continue;

            const QJsonObject step = value.toObject();

            if (!step.contains("step"))
                continue;

            const int stepNumber = step["step"].toInt();

            // -------------------------
            // Step 0
            // -------------------------

            if (stepNumber == 0)
            {
                CommandeStep0 step0;
                step0.reference = step["reference"].toInt();
                step0.reportNumber = step["reportNumber"].toInt();
                step0.customer = step["customer"].toString();
                step0.customerSerialNumber = step["customerSerialNumber"].toString();
                step0.serialNumberAlorem = step["serialNumberAlorem"].toString();
                step0.type = step["type"].toString();
                complete.step0 = step0;
            }

            // -------------------------
            // Step 1
            // -------------------------

            if (stepNumber == 1)
            {
                CommandeStep1 step1;
                step1.reference = step["reference"].toInt();
                if (step.contains("fabricationPlateAdr") && !step["fabricationPlateAdr"].isNull())
                {
                    step1.fabricationPlateAdr = step["fabricationPlateAdr"].toBool();
                }
                if (step.contains("aloremPlate") && !step["aloremPlate"].isNull())
                {
                    step1.aloremPlate = step["aloremPlate"].toBool();
                }
                if (step.contains("bookletPouch") && !step["bookletPouch"].isNull())
                {
                    step1.bookletPouch = step["bookletPouch"].toBool();
                }
                if (step.contains("userManual") && !step["userManual"].isNull())
                {
                    step1.userManual = step["userManual"].toBool();
                }
                if (step.contains("instructionOfUse") && !step["instructionOfUse"].isNull())
                {
                    step1.instructionOfUse = step["instructionOfUse"].toBool();
                }
                if (step.contains("certificatesADR") && !step["certificatesADR"].isNull())
                {
                    step1.certificatesADR = step["certificatesADR"].toBool();
                }
                if (step.contains("groundingAdhesive") && !step["groundingAdhesive"].isNull())
                {
                    step1.groundingAdhesive = step["groundingAdhesive"].toBool();
                }
                if (step.contains("conformityCertificateMarking") && !step["conformityCertificateMarking"].isNull())
                {
                    step1.conformityCertificateMarking = step["conformityCertificateMarking"].toBool();
                }
                complete.step1 = step1;
            }

            // -------------------------
            // Step 2
            // -------------------------

            if (stepNumber == 2)
            {
                CommandeStep2 step2;
                step2.reference = step["reference"].toInt();
                step2.marquePrincipale = step["marquePrincipale"].toString();
                step2.tare = step["tare"].toInt();
                step2.material = step["material"].toString();
                step2.capacity20 = step["capacity20"].toInt();
                step2.grossMass = step["grossMass"].toInt();
                step2.fabricationDate = step["fabricationDate"].toString();
                step2.shellThickness = step["shellThickness"].toInt();
                if (step.contains("pictogramStacking") && !step["pictogramStacking"].isNull())
                {
                    step2.pictogramStacking = step["pictogramStacking"].toBool();
                }
                step2.weightStacking = step["weightStacking"].toInt();
                complete.step2 = step2;
            }

            // -------------------------
            // Step 3
            // -------------------------

            if (stepNumber == 3)
            {
                CommandeStep3 step3;
                step3.reference = step["reference"].toInt();
                if (step.contains("bottomRetentionFace") && !step["bottomRetentionFace"].isNull())
                {
                    step3.bottomRetentionFace = step["bottomRetentionFace"].toInt();
                }
                if (step.contains("bottomRetentionRight") && !step["bottomRetentionRight"].isNull())
                {
                    step3.bottomRetentionRight = step["bottomRetentionRight"].toInt();
                }
                if (step.contains("bottomRetentionLeft") && !step["bottomRetentionLeft"].isNull())
                {
                    step3.bottomRetentionLeft = step["bottomRetentionLeft"].toInt();
                }
                if (step.contains("bottomRetentionBehind") && !step["bottomRetentionBehind"].isNull())
                {
                    step3.bottomRetentionBehind = step["bottomRetentionBehind"].toInt();
                }
                if (step.contains("upperRetention") && !step["upperRetention"].isNull())
                {
                    step3.upperRetention = step["upperRetention"].toInt();
                }
                if (step.contains("liftingRings") && !step["liftingRings"].isNull())
                {
                    step3.liftingRings = step["liftingRings"].toInt();
                }
                if (step.contains("forkliftPass") && !step["forkliftPass"].isNull())
                {
                    step3.forkliftPass = step["forkliftPass"].toInt();
                }
                if (step.contains("dashboard") && !step["dashboard"].isNull())
                {
                    step3.dashboard = step["dashboard"].toInt();
                }
                step3.unauthorizedRepair = step["unauthorizedRepair"].toBool();
                complete.step3 = step3;
            }

            // -------------------------
            // Step 4
            // -------------------------

            if (stepNumber == 4)
            {
                CommandeStep4 step4;
                step4.reference = step["reference"].toInt();
                step4.internalNA = step["internalNA"].toBool();
                if (step.contains("internalOK") && !step["internalOK"].isNull())
                {
                    step4.internalOK = step["internalOK"].toBool();
                }
                step4.internalClean = step["internalClean"].toBool();
                step4.internalObjectInside = step["internalObjectInside"].toBool();
                step4.internalPollution = step["internalPollution"].toBool();
                complete.step4 = step4;
            }

            // -------------------------
            // Step 5
            // -------------------------

            if (stepNumber == 5)
            {
                CommandeStep5 step5;
                step5.reference = step["reference"].toInt();
                step5.epaisseurNA = step["epaisseurNA"].toBool();
                step5.epaisseurMinSideFront = step["epaisseurMinSideFront"].toInt();
                step5.epaisseurMinSideBack = step["epaisseurMinSideBack"].toInt();
                step5.epaisseurMinSideRight = step["epaisseurMinSideRight"].toInt();
                step5.epaisseurMinSideLeft = step["epaisseurMinSideLeft"].toInt();
                step5.epaisseurSideFrontResult1 = step["epaisseurSideFrontResult1"].toInt();
                step5.epaisseurSideFrontResult2 = step["epaisseurSideFrontResult2"].toInt();
                step5.epaisseurSideFrontResult3 = step["epaisseurSideFrontResult3"].toInt();
                step5.epaisseurSideFrontResult4 = step["epaisseurSideFrontResult4"].toInt();
                step5.epaisseurSideFrontResult5 = step["epaisseurSideFrontResult5"].toInt();
                step5.epaisseurSideBackResult1 = step["epaisseurSideBackResult1"].toInt();
                step5.epaisseurSideBackResult2 = step["epaisseurSideBackResult2"].toInt();
                step5.epaisseurSideBackResult3 = step["epaisseurSideBackResult3"].toInt();
                step5.epaisseurSideBackResult4 = step["epaisseurSideBackResult4"].toInt();
                step5.epaisseurSideBackResult5 = step["epaisseurSideBackResult5"].toInt();
                step5.epaisseurSideRightResult1 = step["epaisseurSideRightResult1"].toInt();
                step5.epaisseurSideRightResult2 = step["epaisseurSideRightResult2"].toInt();
                step5.epaisseurSideRightResult3 = step["epaisseurSideRightResult3"].toInt();
                step5.epaisseurSideRightResult4 = step["epaisseurSideRightResult4"].toInt();
                step5.epaisseurSideRightResult5 = step["epaisseurSideRightResult5"].toInt();
                step5.epaisseurSideLeftResult1 = step["epaisseurSideLeftResult1"].toInt();
                step5.epaisseurSideLeftResult2 = step["epaisseurSideLeftResult2"].toInt();
                step5.epaisseurSideLeftResult3 = step["epaisseurSideLeftResult3"].toInt();
                step5.epaisseurSideLeftResult4 = step["epaisseurSideLeftResult4"].toInt();
                step5.epaisseurSideLeftResult5 = step["epaisseurSideLeftResult5"].toInt();
                complete.step5 = step5;
            }

            // -------------------------
            // Step 6
            // -------------------------

            if (stepNumber == 6)
            {
                CommandeStep6 step6;
                step6.reference = step["reference"].toInt();
                if (step.contains("etancheiteOK1") && !step["etancheiteOK1"].isNull())
                {
                    step6.etancheiteOK1 = step["etancheiteOK1"].toBool();
                }
                step6.etancheiteDate1 = step["etancheiteDate1"].toString();
                step6.etancheiteBar1 = static_cast<float>(step["etancheiteBar1"].toDouble());
                if (step.contains("etancheiteOK2") && !step["etancheiteOK2"].isNull())
                {
                    step6.etancheiteOK2 = step["etancheiteOK2"].toBool();
                }
                step6.etancheiteDate2 = step["etancheiteDate2"].toString();
                step6.etancheiteBar2 = static_cast<float>(step["etancheiteBar2"].toDouble());
                complete.step6 = step6;
            }
        }
    }

    return complete;
}

void CommandeController::registerRoutes(QHttpServer &server)
{
    server.route("/api/commandes",
                 QHttpServerRequest::Method::Get,
                 [this]()
                 {
                     QList<Commande> commandes =
                         service.getAllCommandes();

                     QJsonArray jsonArray;

                     for (const Commande &commande : commandes)
                     {
                         QJsonObject jsonCommande;

                         if (commande.uid.has_value())
                             jsonCommande["uid"] = *commande.uid;

                         if (commande.title.has_value())
                             jsonCommande["title"] = *commande.title;

                         if (commande.serialNumber.has_value())
                             jsonCommande["serialNumber"] =
                                 *commande.serialNumber;

                         jsonCommande["currentStep"] =
                             commande.currentStep;

                         jsonCommande["currentlyGoingOn"] =
                             commande.currentlyGoingOn;

                         jsonCommande["loaded"] =
                             commande.loaded;

                         jsonArray.append(jsonCommande);
                     }

                     return QHttpServerResponse(
                         "application/json",
                         QJsonDocument(jsonArray).toJson(
                             QJsonDocument::Compact)
                         );
                 });

    server.route("/api/commandes/<arg>",
                 QHttpServerRequest::Method::Get,
                 [this](const QString &arg)
                 {
                     bool ok = false;

                     int uid = arg.toInt(&ok);

                     if (!ok)
                     {
                         QJsonObject error;
                         error["error"] = "UID invalide";

                         return QHttpServerResponse(
                             "application/json",
                             QJsonDocument(error).toJson(
                                 QJsonDocument::Compact),
                             QHttpServerResponse::StatusCode::BadRequest
                             );
                     }

                     std::optional<CommandeComplete> result =
                         service.getCommandeById(uid);

                     if (!result.has_value())
                     {
                         QJsonObject error;
                         error["error"] = "Commande introuvable";

                         return QHttpServerResponse(
                             "application/json",
                             QJsonDocument(error).toJson(
                                 QJsonDocument::Compact),
                             QHttpServerResponse::StatusCode::NotFound
                             );
                     }

                     const CommandeComplete &commandeComplete =
                         *result;

                     const Commande &commande =
                         commandeComplete.commande;

                     QJsonObject jsonCommande;

                     if (commande.uid.has_value())
                         jsonCommande["uid"] = *commande.uid;

                     if (commande.title.has_value())
                         jsonCommande["title"] = *commande.title;

                     if (commande.serialNumber.has_value())
                         jsonCommande["serialNumber"] =
                             *commande.serialNumber;

                     jsonCommande["currentStep"] =
                         commande.currentStep;

                     jsonCommande["currentlyGoingOn"] =
                         commande.currentlyGoingOn;

                     jsonCommande["loaded"] =
                         commande.loaded;

                     QJsonArray jsonSteps;

                     if (commandeComplete.step0.has_value())
                     {
                         const CommandeStep0 &step =
                             *commandeComplete.step0;

                         QJsonObject jsonStep;

                         jsonStep["step"] = 0;
                         jsonStep["reference"] = step.reference;
                         jsonStep["reportNumber"] = step.reportNumber;
                         jsonStep["customer"] = step.customer;
                         jsonStep["customerSerialNumber"] =
                             step.customerSerialNumber;
                         jsonStep["serialNumberAlorem"] =
                             step.serialNumberAlorem;
                         jsonStep["type"] = step.type;
                         jsonStep["commandeUid"] = step.commandeUid;

                         jsonSteps.append(jsonStep);
                     }

                     if (commandeComplete.step1.has_value())
                     {
                         const CommandeStep1 &step =
                             *commandeComplete.step1;

                         QJsonObject jsonStep;

                         jsonStep["step"] = 1;
                         jsonStep["reference"] = step.reference;
                         jsonStep["fabricationPlateAdr"] =
                             step.fabricationPlateAdr.has_value()
                                 ? QJsonValue(*step.fabricationPlateAdr)
                                 : QJsonValue(QJsonValue::Null);

                         jsonStep["aloremPlate"] =
                             step.aloremPlate.has_value()
                                 ? QJsonValue(*step.aloremPlate)
                                 : QJsonValue(QJsonValue::Null);

                         jsonStep["bookletPouch"] =
                             step.bookletPouch.has_value()
                                 ? QJsonValue(*step.bookletPouch)
                                 : QJsonValue(QJsonValue::Null);

                         jsonStep["userManual"] =
                             step.userManual.has_value()
                                 ? QJsonValue(*step.userManual)
                                 : QJsonValue(QJsonValue::Null);

                         jsonStep["instructionOfUse"] =
                             step.instructionOfUse.has_value()
                                 ? QJsonValue(*step.instructionOfUse)
                                 : QJsonValue(QJsonValue::Null);

                         jsonStep["certificatesADR"] =
                             step.certificatesADR.has_value()
                                 ? QJsonValue(*step.certificatesADR)
                                 : QJsonValue(QJsonValue::Null);

                         jsonStep["groundingAdhesive"] =
                             step.groundingAdhesive.has_value()
                                 ? QJsonValue(*step.groundingAdhesive)
                                 : QJsonValue(QJsonValue::Null);

                         jsonStep["conformityCertificateMarking"] =
                             step.conformityCertificateMarking.has_value()
                                 ? QJsonValue(*step.conformityCertificateMarking)
                                 : QJsonValue(QJsonValue::Null);

                         jsonStep["commandeUid"] = step.commandeUid;

                         jsonSteps.append(jsonStep);
                     }

                     if (commandeComplete.step2.has_value())
                     {
                         const CommandeStep2 &step =
                             *commandeComplete.step2;

                         QJsonObject jsonStep;

                         jsonStep["step"] = 2;
                         jsonStep["reference"] = step.reference;
                         jsonStep["marquePrincipale"] = step.marquePrincipale;
                         jsonStep["tare"] = step.tare;
                         jsonStep["material"] = step.material;
                         jsonStep["capacity20"] = step.capacity20;
                         jsonStep["grossMass"] = step.grossMass;
                         jsonStep["fabricationDate"] = step.fabricationDate;
                         jsonStep["shellThickness"] = step.shellThickness;

                         jsonStep["pictogramStacking"] =
                             step.pictogramStacking.has_value()
                                 ? QJsonValue(*step.pictogramStacking)
                                 : QJsonValue(QJsonValue::Null);

                         jsonStep["weightStacking"] = step.weightStacking;
                         jsonStep["commandeUid"] = step.commandeUid;

                         jsonSteps.append(jsonStep);
                     }

                     if (commandeComplete.step3.has_value())
                     {
                         const CommandeStep3 &step =
                             *commandeComplete.step3;

                         QJsonObject jsonStep;

                         jsonStep["step"] = 3;
                         jsonStep["reference"] = step.reference;

                         jsonStep["bottomRetentionFace"] =
                             step.bottomRetentionFace.has_value()
                                 ? QJsonValue(*step.bottomRetentionFace)
                                 : QJsonValue(QJsonValue::Null);

                         jsonStep["bottomRetentionRight"] =
                             step.bottomRetentionRight.has_value()
                                 ? QJsonValue(*step.bottomRetentionRight)
                                 : QJsonValue(QJsonValue::Null);

                         jsonStep["bottomRetentionLeft"] =
                             step.bottomRetentionLeft.has_value()
                                 ? QJsonValue(*step.bottomRetentionLeft)
                                 : QJsonValue(QJsonValue::Null);

                         jsonStep["bottomRetentionBehind"] =
                             step.bottomRetentionBehind.has_value()
                                 ? QJsonValue(*step.bottomRetentionBehind)
                                 : QJsonValue(QJsonValue::Null);

                         jsonStep["upperRetention"] =
                             step.upperRetention.has_value()
                                 ? QJsonValue(*step.upperRetention)
                                 : QJsonValue(QJsonValue::Null);

                         jsonStep["liftingRings"] =
                             step.liftingRings.has_value()
                                 ? QJsonValue(*step.liftingRings)
                                 : QJsonValue(QJsonValue::Null);

                         jsonStep["forkliftPass"] =
                             step.forkliftPass.has_value()
                                 ? QJsonValue(*step.forkliftPass)
                                 : QJsonValue(QJsonValue::Null);

                         jsonStep["dashboard"] =
                             step.dashboard.has_value()
                                 ? QJsonValue(*step.dashboard)
                                 : QJsonValue(QJsonValue::Null);

                         jsonStep["unauthorizedRepair"] =
                             step.unauthorizedRepair;

                         jsonStep["commandeUid"] =
                             step.commandeUid;

                         jsonSteps.append(jsonStep);
                     }

                     if (commandeComplete.step4.has_value())
                     {
                         const CommandeStep4 &step =
                             *commandeComplete.step4;

                         QJsonObject jsonStep;

                         jsonStep["step"] = 4;
                         jsonStep["reference"] = step.reference;
                         jsonStep["internalNA"] = step.internalNA;

                         jsonStep["internalOK"] =
                             step.internalOK.has_value()
                                 ? QJsonValue(*step.internalOK)
                                 : QJsonValue(QJsonValue::Null);

                         jsonStep["internalClean"] =
                             step.internalClean;

                         jsonStep["internalObjectInside"] =
                             step.internalObjectInside;

                         jsonStep["internalPollution"] =
                             step.internalPollution;

                         jsonStep["commandeUid"] =
                             step.commandeUid;

                         jsonSteps.append(jsonStep);
                     }

                     if (commandeComplete.step5.has_value())
                     {
                         const CommandeStep5 &step =
                             *commandeComplete.step5;

                         QJsonObject jsonStep;

                         jsonStep["step"] = 5;
                         jsonStep["reference"] = step.reference;
                         jsonStep["epaisseurNA"] = step.epaisseurNA;

                         jsonStep["epaisseurMinSideFront"] =
                             step.epaisseurMinSideFront;
                         jsonStep["epaisseurMinSideBack"] =
                             step.epaisseurMinSideBack;
                         jsonStep["epaisseurMinSideRight"] =
                             step.epaisseurMinSideRight;
                         jsonStep["epaisseurMinSideLeft"] =
                             step.epaisseurMinSideLeft;

                         jsonStep["epaisseurSideFrontResult1"] =
                             step.epaisseurSideFrontResult1;
                         jsonStep["epaisseurSideFrontResult2"] =
                             step.epaisseurSideFrontResult2;
                         jsonStep["epaisseurSideFrontResult3"] =
                             step.epaisseurSideFrontResult3;
                         jsonStep["epaisseurSideFrontResult4"] =
                             step.epaisseurSideFrontResult4;
                         jsonStep["epaisseurSideFrontResult5"] =
                             step.epaisseurSideFrontResult5;

                         jsonStep["epaisseurSideBackResult1"] =
                             step.epaisseurSideBackResult1;
                         jsonStep["epaisseurSideBackResult2"] =
                             step.epaisseurSideBackResult2;
                         jsonStep["epaisseurSideBackResult3"] =
                             step.epaisseurSideBackResult3;
                         jsonStep["epaisseurSideBackResult4"] =
                             step.epaisseurSideBackResult4;
                         jsonStep["epaisseurSideBackResult5"] =
                             step.epaisseurSideBackResult5;

                         jsonStep["epaisseurSideRightResult1"] =
                             step.epaisseurSideRightResult1;
                         jsonStep["epaisseurSideRightResult2"] =
                             step.epaisseurSideRightResult2;
                         jsonStep["epaisseurSideRightResult3"] =
                             step.epaisseurSideRightResult3;
                         jsonStep["epaisseurSideRightResult4"] =
                             step.epaisseurSideRightResult4;
                         jsonStep["epaisseurSideRightResult5"] =
                             step.epaisseurSideRightResult5;

                         jsonStep["epaisseurSideLeftResult1"] =
                             step.epaisseurSideLeftResult1;
                         jsonStep["epaisseurSideLeftResult2"] =
                             step.epaisseurSideLeftResult2;
                         jsonStep["epaisseurSideLeftResult3"] =
                             step.epaisseurSideLeftResult3;
                         jsonStep["epaisseurSideLeftResult4"] =
                             step.epaisseurSideLeftResult4;
                         jsonStep["epaisseurSideLeftResult5"] =
                             step.epaisseurSideLeftResult5;

                         jsonStep["commandeUid"] =
                             step.commandeUid;

                         jsonSteps.append(jsonStep);
                     }

                     if (commandeComplete.step6.has_value())
                     {
                         const CommandeStep6 &step =
                             *commandeComplete.step6;

                         QJsonObject jsonStep;

                         jsonStep["step"] = 6;
                         jsonStep["reference"] = step.reference;

                         jsonStep["etancheiteOK1"] =
                             step.etancheiteOK1.has_value()
                                 ? QJsonValue(*step.etancheiteOK1)
                                 : QJsonValue(QJsonValue::Null);

                         jsonStep["etancheiteDate1"] =
                             step.etancheiteDate1;

                         jsonStep["etancheiteBar1"] =
                             step.etancheiteBar1;

                         jsonStep["etancheiteOK2"] =
                             step.etancheiteOK2.has_value()
                                 ? QJsonValue(*step.etancheiteOK2)
                                 : QJsonValue(QJsonValue::Null);

                         jsonStep["etancheiteDate2"] =
                             step.etancheiteDate2;

                         jsonStep["etancheiteBar2"] =
                             step.etancheiteBar2;

                         jsonStep["commandeUid"] =
                             step.commandeUid;

                         jsonSteps.append(jsonStep);
                     }

                     jsonCommande["steps"] = jsonSteps;

                     return QHttpServerResponse(
                         "application/json",
                         QJsonDocument(jsonCommande).toJson(
                             QJsonDocument::Compact)
                         );
                 });

    server.route(
        "/api/commandes",
        QHttpServerRequest::Method::Post,
        [this](const QHttpServerRequest &request)
        {
            // -------------------------
            // Lecture du JSON
            // -------------------------

            const QJsonDocument document =
                QJsonDocument::fromJson(request.body());

            if (document.isNull() ||
                !document.isObject())
            {
                return QHttpServerResponse(
                    "application/json",
                    R"({"error":"JSON invalide"})",
                    QHttpServerResponse::StatusCode::BadRequest);
            }

            const QJsonObject json =
                document.object();

            // -------------------------
            // Parsing
            // -------------------------

            const auto commande =
                parseCommandeComplete(json);

            if (!commande.has_value())
            {
                return QHttpServerResponse(
                    "application/json",
                    R"({"error":"Commande invalide"})",
                    QHttpServerResponse::StatusCode::BadRequest);
            }

            // -------------------------
            // Service
            // -------------------------

            if (!service.createCommande(*commande))
            {
                return QHttpServerResponse(
                    "application/json",
                    R"({"error":"Validation de la commande échouée"})",
                    QHttpServerResponse::StatusCode::BadRequest);
            }

            return QHttpServerResponse(
                "application/json",
                R"({"message":"Commande reçue avec succès"})",
                QHttpServerResponse::StatusCode::Ok);
        });

    server.route(
        "/api/commandes/<arg>",
        QHttpServerRequest::Method::Put,
        [this](const QString &uidString,
               const QHttpServerRequest &request)
        {
            bool ok = false;
            const int uid = uidString.toInt(&ok);

            if (!ok)
            {
                return QHttpServerResponse(
                    "application/json",
                    R"({"error":"UID invalide"})",
                    QHttpServerResponse::StatusCode::BadRequest);
            }

            const QJsonDocument document =
                QJsonDocument::fromJson(request.body());

            if (document.isNull() || !document.isObject())
            {
                return QHttpServerResponse(
                    "application/json",
                    R"({"error":"JSON invalide"})",
                    QHttpServerResponse::StatusCode::BadRequest);
            }

            const QJsonObject json = document.object();

            const auto commande =
                parseCommandeComplete(json);

            if (!commande.has_value())
            {
                return QHttpServerResponse(
                    "application/json",
                    R"({"error":"Commande invalide"})",
                    QHttpServerResponse::StatusCode::BadRequest);
            }

            const auto result =
                service.updateCommande(uid, *commande);

            if (result == UpdateCommandeServiceResult::NotFound)
            {
                return QHttpServerResponse(
                    "application/json",
                    R"({"error":"Commande introuvable"})",
                    QHttpServerResponse::StatusCode::NotFound);
            }

            if (result == UpdateCommandeServiceResult::Error)
            {
                return QHttpServerResponse(
                    "application/json",
                    R"({"error":"Modification de la commande échouée"})",
                    QHttpServerResponse::StatusCode::BadRequest);
            }

            return QHttpServerResponse(
                "application/json",
                R"({"message":"Commande modifiée avec succès"})",
                QHttpServerResponse::StatusCode::Ok);
        });
}