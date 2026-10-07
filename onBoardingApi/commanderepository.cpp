#include "commanderepository.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

QList<Commande> CommandeRepository::findAll()
{
    QList<Commande> commandes;

    QSqlQuery query;

    if (!query.exec(
            "SELECT uid, title, serial_number, "
            "current_step, currently_going_on, loaded "
            "FROM commande "
            "ORDER BY uid"))
    {
        qCritical() << "Erreur SQL :"
                    << query.lastError().text();

        return commandes;
    }

    while (query.next())
    {
        Commande commande;

        if (!query.value("uid").isNull())
            commande.uid = query.value("uid").toInt();

        if (!query.value("title").isNull())
            commande.title = query.value("title").toString();

        if (!query.value("serial_number").isNull())
            commande.serialNumber = query.value("serial_number").toInt();

        commande.currentStep = query.value("current_step").toInt();
        commande.currentlyGoingOn = query.value("currently_going_on").toBool();
        commande.loaded = query.value("loaded").toBool();
        commandes.append(commande);
    }

    return commandes;
}

std::optional<CommandeComplete> CommandeRepository::findById(int uid)
{
    CommandeComplete result;

    // ---------------------------------------------------------
    // Commande
    // ---------------------------------------------------------

    QSqlQuery query;

    query.prepare(
        "SELECT uid, title, serial_number, "
        "current_step, currently_going_on, loaded "
        "FROM commande "
        "WHERE uid = :uid");

    query.bindValue(":uid", uid);

    if (!query.exec())
    {
        qCritical() << "Erreur SQL commande :"
                    << query.lastError().text();

        return std::nullopt;
    }

    if (!query.next())
    {
        // La commande n'existe pas
        return std::nullopt;
    }

    Commande commande;

    if (!query.value("uid").isNull())
        commande.uid = query.value("uid").toInt();

    if (!query.value("title").isNull())
        commande.title = query.value("title").toString();

    if (!query.value("serial_number").isNull())
        commande.serialNumber =
            query.value("serial_number").toInt();

    commande.currentStep = query.value("current_step").toInt();
    commande.currentlyGoingOn = query.value("currently_going_on").toBool();
    commande.loaded = query.value("loaded").toBool();

    result.commande = commande;


    // ---------------------------------------------------------
    // Step 0
    // ---------------------------------------------------------

    query.prepare(
        "SELECT reference, report_number, customer, "
        "customer_serial_number, serial_number_alorem, type "
        "FROM commande_step0 "
        "WHERE commande_uid = :uid");

    query.bindValue(":uid", uid);

    if (!query.exec())
    {
        qCritical() << "Erreur SQL step 0 :"
                    << query.lastError().text();

        return std::nullopt;
    }

    if (query.next())
    {
        CommandeStep0 step;

        step.reference = query.value("reference").toInt();
        step.reportNumber = query.value("report_number").toInt();
        step.customer = query.value("customer").toString();
        step.customerSerialNumber = query.value("customer_serial_number").toString();
        step.serialNumberAlorem = query.value("serial_number_alorem").toString();
        step.type = query.value("type").toString();
        step.commandeUid = uid;

        result.step0 = step;
    }


    // ---------------------------------------------------------
    // Step 1
    // ---------------------------------------------------------

    query.prepare(
        "SELECT reference, fabrication_plate_adr, "
        "alorem_plate, booklet_pouch, user_manual, "
        "instruction_of_use, certificates_adr, "
        "grounding_adhesive, conformity_certificate_marking "
        "FROM commande_step1 "
        "WHERE commande_uid = :uid");

    query.bindValue(":uid", uid);

    if (!query.exec())
    {
        qCritical() << "Erreur SQL step 1 :"
                    << query.lastError().text();

        return std::nullopt;
    }

    if (query.next())
    {
        CommandeStep1 step;

        step.reference = query.value("reference").toInt();
        if (!query.value("fabrication_plate_adr").isNull())
            step.fabricationPlateAdr = query.value("fabrication_plate_adr").toBool();
        if (!query.value("alorem_plate").isNull())
            step.aloremPlate = query.value("alorem_plate").toBool();
        if (!query.value("booklet_pouch").isNull())
            step.bookletPouch = query.value("booklet_pouch").toBool();
        if (!query.value("user_manual").isNull())
            step.userManual = query.value("user_manual").toBool();
        if (!query.value("instruction_of_use").isNull())
            step.instructionOfUse = query.value("instruction_of_use").toBool();
        if (!query.value("certificates_adr").isNull())
            step.certificatesADR = query.value("certificates_adr").toBool();
        if (!query.value("grounding_adhesive").isNull())
            step.groundingAdhesive = query.value("grounding_adhesive").toBool();
        if (!query.value("conformity_certificate_marking").isNull())
            step.conformityCertificateMarking = query.value("conformity_certificate_marking").toBool();
        step.commandeUid = uid;

        result.step1 = step;
    }


    // ---------------------------------------------------------
    // Step 2
    // ---------------------------------------------------------

    query.prepare(
        "SELECT reference, marque_principale, tare, material, "
        "capacity20, gross_mass, fabrication_date, "
        "shell_thickness, pictogram_stacking, weight_stacking "
        "FROM commande_step2 "
        "WHERE commande_uid = :uid");

    query.bindValue(":uid", uid);

    if (!query.exec())
    {
        qCritical() << "Erreur SQL step 2 :"
                    << query.lastError().text();

        return std::nullopt;
    }

    if (query.next())
    {
        CommandeStep2 step;

        step.reference = query.value("reference").toInt();
        step.marquePrincipale = query.value("marque_principale").toString();
        step.tare = query.value("tare").toInt();
        step.material = query.value("material").toString();
        step.capacity20 = query.value("capacity20").toInt();
        step.grossMass = query.value("gross_mass").toInt();
        step.fabricationDate = query.value("fabrication_date").toString();
        step.shellThickness = query.value("shell_thickness").toInt();
        if (!query.value("pictogram_stacking").isNull())
            step.pictogramStacking = query.value("pictogram_stacking").toBool();
        step.weightStacking = query.value("weight_stacking").toInt();
        step.commandeUid = uid;

        result.step2 = step;
    }


    // ---------------------------------------------------------
    // Step 3
    // ---------------------------------------------------------

    query.prepare(
        "SELECT reference, bottom_retention_face, "
        "bottom_retention_right, bottom_retention_left, "
        "bottom_retention_behind, upper_retention, "
        "lifting_rings, forklift_pass, dashboard, "
        "unauthorized_repair "
        "FROM commande_step3 "
        "WHERE commande_uid = :uid");

    query.bindValue(":uid", uid);

    if (!query.exec())
    {
        qCritical() << "Erreur SQL step 3 :"
                    << query.lastError().text();

        return std::nullopt;
    }

    if (query.next())
    {
        CommandeStep3 step;

        step.reference = query.value("reference").toInt();
        if (!query.value("bottom_retention_face").isNull())
            step.bottomRetentionFace = query.value("bottom_retention_face").toInt();
        if (!query.value("bottom_retention_right").isNull())
            step.bottomRetentionRight = query.value("bottom_retention_right").toInt();
        if (!query.value("bottom_retention_left").isNull())
            step.bottomRetentionLeft = query.value("bottom_retention_left").toInt();
        if (!query.value("bottom_retention_behind").isNull())
            step.bottomRetentionBehind = query.value("bottom_retention_behind").toInt();
        if (!query.value("upper_retention").isNull())
            step.upperRetention = query.value("upper_retention").toInt();
        if (!query.value("lifting_rings").isNull())
            step.liftingRings = query.value("lifting_rings").toInt();
        if (!query.value("forklift_pass").isNull())
            step.forkliftPass = query.value("forklift_pass").toInt();
        if (!query.value("dashboard").isNull())
            step.dashboard = query.value("dashboard").toInt();
        step.unauthorizedRepair = query.value("unauthorized_repair").toBool();
        step.commandeUid = uid;

        result.step3 = step;
    }


    // ---------------------------------------------------------
    // Step 4
    // ---------------------------------------------------------

    query.prepare(
        "SELECT reference, internal_na, internal_ok, "
        "internal_clean, internal_object_inside, "
        "internal_pollution "
        "FROM commande_step4 "
        "WHERE commande_uid = :uid");

    query.bindValue(":uid", uid);

    if (!query.exec())
    {
        qCritical() << "Erreur SQL step 4 :"
                    << query.lastError().text();

        return std::nullopt;
    }

    if (query.next())
    {
        CommandeStep4 step;

        step.reference = query.value("reference").toInt();
        step.internalNA = query.value("internal_na").toBool();
        if (!query.value("internal_ok").isNull())
            step.internalOK = query.value("internal_ok").toBool();
        step.internalClean = query.value("internal_clean").toBool();
        step.internalObjectInside = query.value("internal_object_inside").toBool();
        step.internalPollution = query.value("internal_pollution").toBool();
        step.commandeUid = uid;

        result.step4 = step;
    }


    // ---------------------------------------------------------
    // Step 5
    // ---------------------------------------------------------

    query.prepare(
        "SELECT reference, epaisseur_na, "
        "epaisseur_min_side_front, epaisseur_min_side_back, "
        "epaisseur_min_side_right, epaisseur_min_side_left, "

        "epaisseur_side_front_result1, "
        "epaisseur_side_front_result2, "
        "epaisseur_side_front_result3, "
        "epaisseur_side_front_result4, "
        "epaisseur_side_front_result5, "

        "epaisseur_side_back_result1, "
        "epaisseur_side_back_result2, "
        "epaisseur_side_back_result3, "
        "epaisseur_side_back_result4, "
        "epaisseur_side_back_result5, "

        "epaisseur_side_right_result1, "
        "epaisseur_side_right_result2, "
        "epaisseur_side_right_result3, "
        "epaisseur_side_right_result4, "
        "epaisseur_side_right_result5, "

        "epaisseur_side_left_result1, "
        "epaisseur_side_left_result2, "
        "epaisseur_side_left_result3, "
        "epaisseur_side_left_result4, "
        "epaisseur_side_left_result5 "

        "FROM commande_step5 "
        "WHERE commande_uid = :uid");

    query.bindValue(":uid", uid);

    if (!query.exec())
    {
        qCritical() << "Erreur SQL step 5 :"
                    << query.lastError().text();

        return std::nullopt;
    }

    if (query.next())
    {
        CommandeStep5 step;

        step.reference = query.value("reference").toInt();
        step.epaisseurNA = query.value("epaisseur_na").toBool();
        step.epaisseurMinSideFront = query.value("epaisseur_min_side_front").toInt();
        step.epaisseurMinSideBack = query.value("epaisseur_min_side_back").toInt();
        step.epaisseurMinSideRight = query.value("epaisseur_min_side_right").toInt();
        step.epaisseurMinSideLeft = query.value("epaisseur_min_side_left").toInt();
        step.epaisseurSideFrontResult1 = query.value("epaisseur_side_front_result1").toInt();
        step.epaisseurSideFrontResult2 = query.value("epaisseur_side_front_result2").toInt();
        step.epaisseurSideFrontResult3 = query.value("epaisseur_side_front_result3").toInt();
        step.epaisseurSideFrontResult4 = query.value("epaisseur_side_front_result4").toInt();
        step.epaisseurSideFrontResult5 = query.value("epaisseur_side_front_result5").toInt();
        step.epaisseurSideBackResult1 = query.value("epaisseur_side_back_result1").toInt();
        step.epaisseurSideBackResult2 = query.value("epaisseur_side_back_result2").toInt();
        step.epaisseurSideBackResult3 = query.value("epaisseur_side_back_result3").toInt();
        step.epaisseurSideBackResult4 = query.value("epaisseur_side_back_result4").toInt();
        step.epaisseurSideBackResult5 = query.value("epaisseur_side_back_result5").toInt();
        step.epaisseurSideRightResult1 = query.value("epaisseur_side_right_result1").toInt();
        step.epaisseurSideRightResult2 = query.value("epaisseur_side_right_result2").toInt();
        step.epaisseurSideRightResult3 = query.value("epaisseur_side_right_result3").toInt();
        step.epaisseurSideRightResult4 = query.value("epaisseur_side_right_result4").toInt();
        step.epaisseurSideRightResult5 = query.value("epaisseur_side_right_result5").toInt();
        step.epaisseurSideLeftResult1 = query.value("epaisseur_side_left_result1").toInt();
        step.epaisseurSideLeftResult2 = query.value("epaisseur_side_left_result2").toInt();
        step.epaisseurSideLeftResult3 = query.value("epaisseur_side_left_result3").toInt();
        step.epaisseurSideLeftResult4 = query.value("epaisseur_side_left_result4").toInt();
        step.epaisseurSideLeftResult5 = query.value("epaisseur_side_left_result5").toInt();
        step.commandeUid = uid;

        result.step5 = step;
    }


    // ---------------------------------------------------------
    // Step 6
    // ---------------------------------------------------------

    query.prepare(
        "SELECT reference, etancheite_ok1, etancheite_date1, "
        "etancheite_bar1, etancheite_ok2, etancheite_date2, "
        "etancheite_bar2 "
        "FROM commande_step6 "
        "WHERE commande_uid = :uid");

    query.bindValue(":uid", uid);

    if (!query.exec())
    {
        qCritical() << "Erreur SQL step 6 :"
                    << query.lastError().text();

        return std::nullopt;
    }

    if (query.next())
    {
        CommandeStep6 step;

        step.reference = query.value("reference").toInt();
        if (!query.value("etancheite_ok1").isNull())
            step.etancheiteOK1 = query.value("etancheite_ok1").toBool();
        step.etancheiteDate1 = query.value("etancheite_date1").toString();
        step.etancheiteBar1 = query.value("etancheite_bar1").toFloat();
        if (!query.value("etancheite_ok2").isNull())
            step.etancheiteOK2 = query.value("etancheite_ok2").toBool();
        step.etancheiteDate2 = query.value("etancheite_date2").toString();
        step.etancheiteBar2 = query.value("etancheite_bar2").toFloat();
        step.commandeUid = uid;

        result.step6 = step;
    }

    return result;
}

std::optional<int> CommandeRepository::createCommande(const CommandeComplete &commande)
{
    QSqlDatabase db = QSqlDatabase::database();

    if (!db.transaction())
    {
        qCritical()
        << "Impossible de démarrer la transaction :"
        << db.lastError().text();

        return std::nullopt;
    }

    QSqlQuery query;

    query.prepare(
        "INSERT INTO commande "
        "(title, serial_number, current_step, "
        "currently_going_on, loaded) "
        "VALUES "
        "(:title, :serial_number, :current_step, "
        ":currently_going_on, :loaded) "
        "RETURNING uid"
        );

    if (commande.commande.title.has_value())
        query.bindValue(":title", *commande.commande.title);
    else
        query.bindValue(":title", QVariant());

    if (commande.commande.serialNumber.has_value())
        query.bindValue(":serial_number", *commande.commande.serialNumber);
    else
        query.bindValue(":serial_number", QVariant());

    query.bindValue(":current_step",commande.commande.currentStep);
    query.bindValue(":currently_going_on",commande.commande.currentlyGoingOn);
    query.bindValue(":loaded",commande.commande.loaded);

    if (!query.exec())
    {
        qCritical() << "Erreur SQL création commande :"
                    << query.lastError().text();

        db.rollback();

        return std::nullopt;
    }

    if (!query.next())
    {
        qCritical()
        << "Erreur : PostgreSQL n'a pas retourné le uid";

        db.rollback();

        return std::nullopt;
    }

    const int uid = query.value("uid").toInt();

    if (commande.step0.has_value())
    {
        QSqlQuery step0Query;

        step0Query.prepare(
            "INSERT INTO commande_step0 "
            "(commande_uid, reference, report_number, customer, "
            "customer_serial_number, serial_number_alorem, type) "
            "VALUES "
            "(:commande_uid, :reference, :report_number, :customer, "
            ":customer_serial_number, :serial_number_alorem, :type)"
            );

        step0Query.bindValue(":commande_uid", uid);
        step0Query.bindValue(":reference", commande.step0->reference);
        step0Query.bindValue(":report_number", commande.step0->reportNumber);
        step0Query.bindValue(":customer", commande.step0->customer);
        step0Query.bindValue(":customer_serial_number", commande.step0->customerSerialNumber);
        step0Query.bindValue(":serial_number_alorem", commande.step0->serialNumberAlorem);
        step0Query.bindValue(":type", commande.step0->type);

        if (!step0Query.exec())
        {
            qCritical()
            << "Erreur SQL création step 0 :"
            << step0Query.lastError().text();

            db.rollback();

            return std::nullopt;
        }

        qInfo()
            << "Step 0 créé pour la commande :"
            << uid;
    }

    if (commande.step1.has_value())
    {
        QSqlQuery step1Query;

        step1Query.prepare(
            "INSERT INTO commande_step1 "
            "(commande_uid, reference, fabrication_plate_adr, "
            "alorem_plate, booklet_pouch, user_manual, "
            "instruction_of_use, certificates_adr, "
            "grounding_adhesive, conformity_certificate_marking) "
            "VALUES "
            "(:commande_uid, :reference, :fabrication_plate_adr, "
            ":alorem_plate, :booklet_pouch, :user_manual, "
            ":instruction_of_use, :certificates_adr, "
            ":grounding_adhesive, :conformity_certificate_marking)"
            );

        step1Query.bindValue(":commande_uid", uid);
        step1Query.bindValue(":reference", commande.step1->reference);

        if (commande.step1->fabricationPlateAdr.has_value())
            step1Query.bindValue(":fabrication_plate_adr", *commande.step1->fabricationPlateAdr);
        else
            step1Query.bindValue(":fabrication_plate_adr", QVariant());

        if (commande.step1->aloremPlate.has_value())
            step1Query.bindValue(":alorem_plate", *commande.step1->aloremPlate);
        else
            step1Query.bindValue(":alorem_plate", QVariant());

        if (commande.step1->bookletPouch.has_value())
            step1Query.bindValue(":booklet_pouch", *commande.step1->bookletPouch);
        else
            step1Query.bindValue(":booklet_pouch", QVariant());

        if (commande.step1->userManual.has_value())
            step1Query.bindValue(":user_manual", *commande.step1->userManual);
        else
            step1Query.bindValue(":user_manual", QVariant());

        if (commande.step1->instructionOfUse.has_value())
            step1Query.bindValue(":instruction_of_use", *commande.step1->instructionOfUse);
        else
            step1Query.bindValue(":instruction_of_use", QVariant());

        if (commande.step1->certificatesADR.has_value())
            step1Query.bindValue(":certificates_adr", *commande.step1->certificatesADR);
        else
            step1Query.bindValue(":certificates_adr", QVariant());

        if (commande.step1->groundingAdhesive.has_value())
            step1Query.bindValue(":grounding_adhesive", *commande.step1->groundingAdhesive);
        else
            step1Query.bindValue(":grounding_adhesive", QVariant());

        if (commande.step1->conformityCertificateMarking.has_value())
            step1Query.bindValue(":conformity_certificate_marking", *commande.step1->conformityCertificateMarking);
        else
            step1Query.bindValue(":conformity_certificate_marking", QVariant());

        if (!step1Query.exec())
        {
            qCritical()
            << "Erreur SQL création step 1 :"
            << step1Query.lastError().text();

            db.rollback();

            return std::nullopt;
        }

        qInfo()
            << "Step 1 créé pour la commande :"
            << uid;
    }

    if (commande.step2.has_value())
    {
        QSqlQuery step2Query;

        step2Query.prepare(
            "INSERT INTO commande_step2 "
            "(commande_uid, reference, marque_principale, tare, "
            "material, capacity20, gross_mass, fabrication_date, "
            "shell_thickness, pictogram_stacking, weight_stacking) "
            "VALUES "
            "(:commande_uid, :reference, :marque_principale, :tare, "
            ":material, :capacity20, :gross_mass, :fabrication_date, "
            ":shell_thickness, :pictogram_stacking, :weight_stacking)"
            );

        step2Query.bindValue(":commande_uid", uid);
        step2Query.bindValue(":reference", commande.step2->reference);
        step2Query.bindValue(":marque_principale", commande.step2->marquePrincipale);
        step2Query.bindValue(":tare", commande.step2->tare);
        step2Query.bindValue(":material", commande.step2->material);
        step2Query.bindValue(":capacity20", commande.step2->capacity20);
        step2Query.bindValue(":gross_mass", commande.step2->grossMass);
        step2Query.bindValue(":fabrication_date", commande.step2->fabricationDate);
        step2Query.bindValue(":shell_thickness", commande.step2->shellThickness);

        if (commande.step2->pictogramStacking.has_value())
            step2Query.bindValue(":pictogram_stacking", *commande.step2->pictogramStacking);
        else
            step2Query.bindValue(":pictogram_stacking", QVariant());

        step2Query.bindValue(":weight_stacking", commande.step2->weightStacking);

        if (!step2Query.exec())
        {
            qCritical()
            << "Erreur SQL création step 2 :"
            << step2Query.lastError().text();

            db.rollback();

            return std::nullopt;
        }

        qInfo()
            << "Step 2 créé pour la commande :"
            << uid;
    }

    if (commande.step3.has_value())
    {
        QSqlQuery step3Query;

        step3Query.prepare(
            "INSERT INTO commande_step3 "
            "(commande_uid, reference, "
            "bottom_retention_face, bottom_retention_right, "
            "bottom_retention_left, bottom_retention_behind, "
            "upper_retention, lifting_rings, forklift_pass, "
            "dashboard, unauthorized_repair) "
            "VALUES "
            "(:commande_uid, :reference, "
            ":bottom_retention_face, :bottom_retention_right, "
            ":bottom_retention_left, :bottom_retention_behind, "
            ":upper_retention, :lifting_rings, :forklift_pass, "
            ":dashboard, :unauthorized_repair)"
            );

        step3Query.bindValue(":commande_uid", uid);
        step3Query.bindValue(":reference", commande.step3->reference);

        if (commande.step3->bottomRetentionFace.has_value())
            step3Query.bindValue(":bottom_retention_face", *commande.step3->bottomRetentionFace);
        else
            step3Query.bindValue(":bottom_retention_face", QVariant());

        if (commande.step3->bottomRetentionRight.has_value())
            step3Query.bindValue(":bottom_retention_right", *commande.step3->bottomRetentionRight);
        else
            step3Query.bindValue(":bottom_retention_right", QVariant());

        if (commande.step3->bottomRetentionLeft.has_value())
            step3Query.bindValue(":bottom_retention_left", *commande.step3->bottomRetentionLeft);
        else
            step3Query.bindValue(":bottom_retention_left", QVariant());

        if (commande.step3->bottomRetentionBehind.has_value())
            step3Query.bindValue(":bottom_retention_behind", *commande.step3->bottomRetentionBehind);
        else
            step3Query.bindValue(":bottom_retention_behind", QVariant());

        if (commande.step3->upperRetention.has_value())
            step3Query.bindValue(":upper_retention", *commande.step3->upperRetention);
        else
            step3Query.bindValue(":upper_retention", QVariant());

        if (commande.step3->liftingRings.has_value())
            step3Query.bindValue(":lifting_rings", *commande.step3->liftingRings);
        else
            step3Query.bindValue(":lifting_rings", QVariant());

        if (commande.step3->forkliftPass.has_value())
            step3Query.bindValue(":forklift_pass", *commande.step3->forkliftPass);
        else
            step3Query.bindValue(":forklift_pass", QVariant());

        if (commande.step3->dashboard.has_value())
            step3Query.bindValue(":dashboard", *commande.step3->dashboard);
        else
            step3Query.bindValue(":dashboard", QVariant());

        step3Query.bindValue(":unauthorized_repair", commande.step3->unauthorizedRepair);

        if (!step3Query.exec())
        {
            qCritical()
            << "Erreur SQL création step 3 :"
            << step3Query.lastError().text();

            db.rollback();

            return std::nullopt;
        }

        qInfo()
            << "Step 3 créé pour la commande :"
            << uid;
    }

    if (commande.step4.has_value())
    {
        QSqlQuery step4Query;

        step4Query.prepare(
            "INSERT INTO commande_step4 "
            "(commande_uid, reference, internal_na, internal_ok, "
            "internal_clean, internal_object_inside, internal_pollution) "
            "VALUES "
            "(:commande_uid, :reference, :internal_na, :internal_ok, "
            ":internal_clean, :internal_object_inside, :internal_pollution)"
            );

        step4Query.bindValue(":commande_uid", uid);
        step4Query.bindValue(":reference", commande.step4->reference);
        step4Query.bindValue(":internal_na", commande.step4->internalNA);

        if (commande.step4->internalOK.has_value())
            step4Query.bindValue(":internal_ok", *commande.step4->internalOK);
        else
            step4Query.bindValue(":internal_ok", QVariant());

        step4Query.bindValue(":internal_clean", commande.step4->internalClean);
        step4Query.bindValue(":internal_object_inside", commande.step4->internalObjectInside);
        step4Query.bindValue(":internal_pollution", commande.step4->internalPollution);

        if (!step4Query.exec())
        {
            qCritical()
            << "Erreur SQL création step 4 :"
            << step4Query.lastError().text();

            db.rollback();

            return std::nullopt;
        }

        qInfo()
            << "Step 4 créé pour la commande :"
            << uid;
    }

    if (commande.step5.has_value())
    {
        QSqlQuery step5Query;

        step5Query.prepare(
            "INSERT INTO commande_step5 "
            "(commande_uid, reference, epaisseur_na, "
            "epaisseur_min_side_front, epaisseur_min_side_back, "
            "epaisseur_min_side_right, epaisseur_min_side_left, "
            "epaisseur_side_front_result1, epaisseur_side_front_result2, "
            "epaisseur_side_front_result3, epaisseur_side_front_result4, "
            "epaisseur_side_front_result5, "
            "epaisseur_side_back_result1, epaisseur_side_back_result2, "
            "epaisseur_side_back_result3, epaisseur_side_back_result4, "
            "epaisseur_side_back_result5, "
            "epaisseur_side_right_result1, epaisseur_side_right_result2, "
            "epaisseur_side_right_result3, epaisseur_side_right_result4, "
            "epaisseur_side_right_result5, "
            "epaisseur_side_left_result1, epaisseur_side_left_result2, "
            "epaisseur_side_left_result3, epaisseur_side_left_result4, "
            "epaisseur_side_left_result5) "
            "VALUES "
            "(:commande_uid, :reference, :epaisseur_na, "
            ":epaisseur_min_side_front, :epaisseur_min_side_back, "
            ":epaisseur_min_side_right, :epaisseur_min_side_left, "
            ":epaisseur_side_front_result1, :epaisseur_side_front_result2, "
            ":epaisseur_side_front_result3, :epaisseur_side_front_result4, "
            ":epaisseur_side_front_result5, "
            ":epaisseur_side_back_result1, :epaisseur_side_back_result2, "
            ":epaisseur_side_back_result3, :epaisseur_side_back_result4, "
            ":epaisseur_side_back_result5, "
            ":epaisseur_side_right_result1, :epaisseur_side_right_result2, "
            ":epaisseur_side_right_result3, :epaisseur_side_right_result4, "
            ":epaisseur_side_right_result5, "
            ":epaisseur_side_left_result1, :epaisseur_side_left_result2, "
            ":epaisseur_side_left_result3, :epaisseur_side_left_result4, "
            ":epaisseur_side_left_result5)"
            );

        step5Query.bindValue(":commande_uid", uid);
        step5Query.bindValue(":reference", commande.step5->reference);
        step5Query.bindValue(":epaisseur_na", commande.step5->epaisseurNA);
        step5Query.bindValue(":epaisseur_min_side_front", commande.step5->epaisseurMinSideFront);
        step5Query.bindValue(":epaisseur_min_side_back", commande.step5->epaisseurMinSideBack);
        step5Query.bindValue(":epaisseur_min_side_right", commande.step5->epaisseurMinSideRight);
        step5Query.bindValue(":epaisseur_min_side_left", commande.step5->epaisseurMinSideLeft);
        step5Query.bindValue(":epaisseur_side_front_result1", commande.step5->epaisseurSideFrontResult1);
        step5Query.bindValue(":epaisseur_side_front_result2", commande.step5->epaisseurSideFrontResult2);
        step5Query.bindValue(":epaisseur_side_front_result3", commande.step5->epaisseurSideFrontResult3);
        step5Query.bindValue(":epaisseur_side_front_result4", commande.step5->epaisseurSideFrontResult4);
        step5Query.bindValue(":epaisseur_side_front_result5", commande.step5->epaisseurSideFrontResult5);
        step5Query.bindValue(":epaisseur_side_back_result1", commande.step5->epaisseurSideBackResult1);
        step5Query.bindValue(":epaisseur_side_back_result2", commande.step5->epaisseurSideBackResult2);
        step5Query.bindValue(":epaisseur_side_back_result3", commande.step5->epaisseurSideBackResult3);
        step5Query.bindValue(":epaisseur_side_back_result4", commande.step5->epaisseurSideBackResult4);
        step5Query.bindValue(":epaisseur_side_back_result5", commande.step5->epaisseurSideBackResult5);
        step5Query.bindValue(":epaisseur_side_right_result1", commande.step5->epaisseurSideRightResult1);
        step5Query.bindValue(":epaisseur_side_right_result2", commande.step5->epaisseurSideRightResult2);
        step5Query.bindValue(":epaisseur_side_right_result3", commande.step5->epaisseurSideRightResult3);
        step5Query.bindValue(":epaisseur_side_right_result4", commande.step5->epaisseurSideRightResult4);
        step5Query.bindValue(":epaisseur_side_right_result5", commande.step5->epaisseurSideRightResult5);
        step5Query.bindValue(":epaisseur_side_left_result1", commande.step5->epaisseurSideLeftResult1);
        step5Query.bindValue(":epaisseur_side_left_result2", commande.step5->epaisseurSideLeftResult2);
        step5Query.bindValue(":epaisseur_side_left_result3", commande.step5->epaisseurSideLeftResult3);
        step5Query.bindValue(":epaisseur_side_left_result4", commande.step5->epaisseurSideLeftResult4);
        step5Query.bindValue(":epaisseur_side_left_result5", commande.step5->epaisseurSideLeftResult5);

        if (!step5Query.exec())
        {
            qCritical()
            << "Erreur SQL création step 5 :"
            << step5Query.lastError().text();

            db.rollback();

            return std::nullopt;
        }

        qInfo()
            << "Step 5 créé pour la commande :"
            << uid;
    }

    if (commande.step6.has_value())
    {
        QSqlQuery step6Query;

        step6Query.prepare(
            "INSERT INTO commande_step6 "
            "(commande_uid, reference, "
            "etancheite_ok1, etancheite_date1, etancheite_bar1, "
            "etancheite_ok2, etancheite_date2, etancheite_bar2) "
            "VALUES "
            "(:commande_uid, :reference, "
            ":etancheite_ok1, :etancheite_date1, :etancheite_bar1, "
            ":etancheite_ok2, :etancheite_date2, :etancheite_bar2)"
            );

        step6Query.bindValue(":commande_uid", uid);
        step6Query.bindValue(":reference", commande.step6->reference);

        if (commande.step6->etancheiteOK1.has_value())
            step6Query.bindValue(":etancheite_ok1", *commande.step6->etancheiteOK1);
        else
            step6Query.bindValue(":etancheite_ok1", QVariant());

        step6Query.bindValue(":etancheite_date1", commande.step6->etancheiteDate1);
        step6Query.bindValue(":etancheite_bar1", commande.step6->etancheiteBar1);

        if (commande.step6->etancheiteOK2.has_value())
            step6Query.bindValue(":etancheite_ok2", *commande.step6->etancheiteOK2);
        else
            step6Query.bindValue(":etancheite_ok2", QVariant());

        step6Query.bindValue(":etancheite_date2", commande.step6->etancheiteDate2);
        step6Query.bindValue(":etancheite_bar2", commande.step6->etancheiteBar2);

        if (!step6Query.exec())
        {
            qCritical()
            << "Erreur SQL création step 6 :"
            << step6Query.lastError().text();

            db.rollback();

            return std::nullopt;
        }

        qInfo()
            << "Step 6 créé pour la commande :"
            << uid;
    }

    if (!db.commit())
    {
        qCritical()
        << "Erreur lors du commit de la commande :"
        << db.lastError().text();

        db.rollback();

        return std::nullopt;
    }

    qInfo() << "Commande créée avec uid :" << uid;

    return uid;
}

UpdateCommandeResult  CommandeRepository::updateCommande(int uid, const CommandeComplete &commande)
{
    QSqlDatabase db = QSqlDatabase::database();

    if (!db.transaction())
    {
        qCritical()
        << "Impossible de démarrer la transaction :"
        << db.lastError().text();

        return UpdateCommandeResult::Error;
    }

    QSqlQuery query;

    query.prepare(
        "UPDATE commande "
        "SET title = :title, "
        "serial_number = :serial_number, "
        "current_step = :current_step, "
        "currently_going_on = :currently_going_on, "
        "loaded = :loaded "
        "WHERE uid = :uid"
        );

    if (commande.commande.title.has_value())
        query.bindValue(":title", *commande.commande.title);
    else
        query.bindValue(":title", QVariant());

    if (commande.commande.serialNumber.has_value())
        query.bindValue(":serial_number", *commande.commande.serialNumber);
    else
        query.bindValue(":serial_number", QVariant());

    query.bindValue(":current_step", commande.commande.currentStep);
    query.bindValue(":currently_going_on", commande.commande.currentlyGoingOn);
    query.bindValue(":loaded", commande.commande.loaded);
    query.bindValue(":uid", uid);

    if (!query.exec())
    {
        qCritical()
        << "Erreur SQL modification commande :"
        << query.lastError().text();

        db.rollback();
        return UpdateCommandeResult::Error;
    }

    if (query.numRowsAffected() == 0)
    {
        qWarning()
        << "Aucune commande trouvée avec uid :"
        << uid;

        db.rollback();
        return UpdateCommandeResult::NotFound;
    }

    if (commande.step0.has_value())
    {
        QSqlQuery stepQuery;

        stepQuery.prepare(
            "UPDATE commande_step0 "
            "SET reference = :reference, "
            "report_number = :report_number, "
            "customer = :customer, "
            "customer_serial_number = :customer_serial_number, "
            "serial_number_alorem = :serial_number_alorem, "
            "type = :type "
            "WHERE commande_uid = :commande_uid"
            );

        const auto &step = *commande.step0;

        stepQuery.bindValue(":reference", step.reference);
        stepQuery.bindValue(":report_number", step.reportNumber);
        stepQuery.bindValue(":customer", step.customer);
        stepQuery.bindValue(
            ":customer_serial_number",
            step.customerSerialNumber);
        stepQuery.bindValue(
            ":serial_number_alorem",
            step.serialNumberAlorem);
        stepQuery.bindValue(":type", step.type);
        stepQuery.bindValue(":commande_uid", uid);

        if (!stepQuery.exec())
        {
            qCritical()
            << "Erreur SQL modification step0 :"
            << stepQuery.lastError().text();

            db.rollback();
            return UpdateCommandeResult::Error;
        }

        if (stepQuery.numRowsAffected() == 0)
        {
            QSqlQuery insertQuery;

            insertQuery.prepare(
                "INSERT INTO commande_step0 "
                "(commande_uid, reference, report_number, customer, "
                "customer_serial_number, serial_number_alorem, type) "
                "VALUES "
                "(:commande_uid, :reference, :report_number, :customer, "
                ":customer_serial_number, :serial_number_alorem, :type)"
                );

            insertQuery.bindValue(":commande_uid", uid);
            insertQuery.bindValue(":reference", step.reference);
            insertQuery.bindValue(":report_number", step.reportNumber);
            insertQuery.bindValue(":customer", step.customer);
            insertQuery.bindValue(
                ":customer_serial_number",
                step.customerSerialNumber);
            insertQuery.bindValue(
                ":serial_number_alorem",
                step.serialNumberAlorem);
            insertQuery.bindValue(":type", step.type);

            if (!insertQuery.exec())
            {
                qCritical()
                << "Erreur SQL insertion step0 :"
                << insertQuery.lastError().text();

                db.rollback();
                return UpdateCommandeResult::Error;
            }
        }
    }

    if (commande.step1.has_value())
    {
        QSqlQuery stepQuery;

        stepQuery.prepare(
            "UPDATE commande_step1 "
            "SET reference = :reference, "
            "fabrication_plate_adr = :fabrication_plate_adr, "
            "alorem_plate = :alorem_plate, "
            "booklet_pouch = :booklet_pouch, "
            "user_manual = :user_manual, "
            "instruction_of_use = :instruction_of_use, "
            "certificates_adr = :certificates_adr, "
            "grounding_adhesive = :grounding_adhesive, "
            "conformity_certificate_marking = :conformity_certificate_marking "
            "WHERE commande_uid = :commande_uid"
            );

        const auto &step = *commande.step1;

        stepQuery.bindValue(":reference", step.reference);

        if (step.fabricationPlateAdr.has_value())
            stepQuery.bindValue(":fabrication_plate_adr", *step.fabricationPlateAdr);
        else
            stepQuery.bindValue(":fabrication_plate_adr", QVariant());

        if (step.aloremPlate.has_value())
            stepQuery.bindValue(":alorem_plate", *step.aloremPlate);
        else
            stepQuery.bindValue(":alorem_plate", QVariant());

        if (step.bookletPouch.has_value())
            stepQuery.bindValue(":booklet_pouch", *step.bookletPouch);
        else
            stepQuery.bindValue(":booklet_pouch", QVariant());

        if (step.userManual.has_value())
            stepQuery.bindValue(":user_manual", *step.userManual);
        else
            stepQuery.bindValue(":user_manual", QVariant());

        if (step.instructionOfUse.has_value())
            stepQuery.bindValue(":instruction_of_use", *step.instructionOfUse);
        else
            stepQuery.bindValue(":instruction_of_use", QVariant());

        if (step.certificatesADR.has_value())
            stepQuery.bindValue(":certificates_adr", *step.certificatesADR);
        else
            stepQuery.bindValue(":certificates_adr", QVariant());

        if (step.groundingAdhesive.has_value())
            stepQuery.bindValue(":grounding_adhesive", *step.groundingAdhesive);
        else
            stepQuery.bindValue(":grounding_adhesive", QVariant());

        if (step.conformityCertificateMarking.has_value())
            stepQuery.bindValue(
                ":conformity_certificate_marking",
                *step.conformityCertificateMarking);
        else
            stepQuery.bindValue(
                ":conformity_certificate_marking",
                QVariant());

        stepQuery.bindValue(":commande_uid", uid);

        if (!stepQuery.exec())
        {
            qCritical()
            << "Erreur SQL modification step1 :"
            << stepQuery.lastError().text();

            db.rollback();
            return UpdateCommandeResult::Error;
        }

        if (stepQuery.numRowsAffected() == 0)
        {
            QSqlQuery insertQuery;

            insertQuery.prepare(
                "INSERT INTO commande_step1 "
                "(commande_uid, reference, fabrication_plate_adr, "
                "alorem_plate, booklet_pouch, user_manual, "
                "instruction_of_use, certificates_adr, "
                "grounding_adhesive, conformity_certificate_marking) "
                "VALUES "
                "(:commande_uid, :reference, :fabrication_plate_adr, "
                ":alorem_plate, :booklet_pouch, :user_manual, "
                ":instruction_of_use, :certificates_adr, "
                ":grounding_adhesive, :conformity_certificate_marking)"
                );

            insertQuery.bindValue(":commande_uid", uid);
            insertQuery.bindValue(":reference", step.reference);

            if (step.fabricationPlateAdr.has_value())
                insertQuery.bindValue(
                    ":fabrication_plate_adr",
                    *step.fabricationPlateAdr);
            else
                insertQuery.bindValue(
                    ":fabrication_plate_adr",
                    QVariant());

            if (step.aloremPlate.has_value())
                insertQuery.bindValue(":alorem_plate", *step.aloremPlate);
            else
                insertQuery.bindValue(":alorem_plate", QVariant());

            if (step.bookletPouch.has_value())
                insertQuery.bindValue(":booklet_pouch", *step.bookletPouch);
            else
                insertQuery.bindValue(":booklet_pouch", QVariant());

            if (step.userManual.has_value())
                insertQuery.bindValue(":user_manual", *step.userManual);
            else
                insertQuery.bindValue(":user_manual", QVariant());

            if (step.instructionOfUse.has_value())
                insertQuery.bindValue(
                    ":instruction_of_use",
                    *step.instructionOfUse);
            else
                insertQuery.bindValue(
                    ":instruction_of_use",
                    QVariant());

            if (step.certificatesADR.has_value())
                insertQuery.bindValue(
                    ":certificates_adr",
                    *step.certificatesADR);
            else
                insertQuery.bindValue(
                    ":certificates_adr",
                    QVariant());

            if (step.groundingAdhesive.has_value())
                insertQuery.bindValue(
                    ":grounding_adhesive",
                    *step.groundingAdhesive);
            else
                insertQuery.bindValue(
                    ":grounding_adhesive",
                    QVariant());

            if (step.conformityCertificateMarking.has_value())
                insertQuery.bindValue(
                    ":conformity_certificate_marking",
                    *step.conformityCertificateMarking);
            else
                insertQuery.bindValue(
                    ":conformity_certificate_marking",
                    QVariant());

            if (!insertQuery.exec())
            {
                qCritical()
                << "Erreur SQL insertion step1 :"
                << insertQuery.lastError().text();

                db.rollback();
                return UpdateCommandeResult::Error;
            }
        }
    }

    if (commande.step2.has_value())
    {
        QSqlQuery stepQuery;

        stepQuery.prepare(
            "UPDATE commande_step2 "
            "SET reference = :reference, "
            "marque_principale = :marque_principale, "
            "tare = :tare, "
            "material = :material, "
            "capacity20 = :capacity20, "
            "gross_mass = :gross_mass, "
            "fabrication_date = :fabrication_date, "
            "shell_thickness = :shell_thickness, "
            "pictogram_stacking = :pictogram_stacking, "
            "weight_stacking = :weight_stacking "
            "WHERE commande_uid = :commande_uid"
            );

        const auto &step = *commande.step2;

        stepQuery.bindValue(":reference", step.reference);
        stepQuery.bindValue(":marque_principale", step.marquePrincipale);
        stepQuery.bindValue(":tare", step.tare);
        stepQuery.bindValue(":material", step.material);
        stepQuery.bindValue(":capacity20", step.capacity20);
        stepQuery.bindValue(":gross_mass", step.grossMass);
        stepQuery.bindValue(":fabrication_date", step.fabricationDate);
        stepQuery.bindValue(":shell_thickness", step.shellThickness);

        if (step.pictogramStacking.has_value())
            stepQuery.bindValue(
                ":pictogram_stacking",
                *step.pictogramStacking);
        else
            stepQuery.bindValue(
                ":pictogram_stacking",
                QVariant());

        stepQuery.bindValue(":weight_stacking", step.weightStacking);
        stepQuery.bindValue(":commande_uid", uid);

        if (!stepQuery.exec())
        {
            qCritical()
            << "Erreur SQL modification step2 :"
            << stepQuery.lastError().text();

            db.rollback();
            return UpdateCommandeResult::Error;
        }

        if (stepQuery.numRowsAffected() == 0)
        {
            QSqlQuery insertQuery;

            insertQuery.prepare(
                "INSERT INTO commande_step2 "
                "(commande_uid, reference, marque_principale, tare, "
                "material, capacity20, gross_mass, fabrication_date, "
                "shell_thickness, pictogram_stacking, weight_stacking) "
                "VALUES "
                "(:commande_uid, :reference, :marque_principale, :tare, "
                ":material, :capacity20, :gross_mass, :fabrication_date, "
                ":shell_thickness, :pictogram_stacking, :weight_stacking)"
                );

            insertQuery.bindValue(":commande_uid", uid);
            insertQuery.bindValue(":reference", step.reference);
            insertQuery.bindValue(
                ":marque_principale",
                step.marquePrincipale);
            insertQuery.bindValue(":tare", step.tare);
            insertQuery.bindValue(":material", step.material);
            insertQuery.bindValue(":capacity20", step.capacity20);
            insertQuery.bindValue(":gross_mass", step.grossMass);
            insertQuery.bindValue(
                ":fabrication_date",
                step.fabricationDate);
            insertQuery.bindValue(
                ":shell_thickness",
                step.shellThickness);

            if (step.pictogramStacking.has_value())
                insertQuery.bindValue(
                    ":pictogram_stacking",
                    *step.pictogramStacking);
            else
                insertQuery.bindValue(
                    ":pictogram_stacking",
                    QVariant());

            insertQuery.bindValue(
                ":weight_stacking",
                step.weightStacking);

            if (!insertQuery.exec())
            {
                qCritical()
                << "Erreur SQL insertion step2 :"
                << insertQuery.lastError().text();

                db.rollback();
                return UpdateCommandeResult::Error;
            }
        }
    }

    if (commande.step3.has_value())
    {
        QSqlQuery stepQuery;

        stepQuery.prepare(
            "UPDATE commande_step3 "
            "SET reference = :reference, "
            "bottom_retention_face = :bottom_retention_face, "
            "bottom_retention_right = :bottom_retention_right, "
            "bottom_retention_left = :bottom_retention_left, "
            "bottom_retention_behind = :bottom_retention_behind, "
            "upper_retention = :upper_retention, "
            "lifting_rings = :lifting_rings, "
            "forklift_pass = :forklift_pass, "
            "dashboard = :dashboard, "
            "unauthorized_repair = :unauthorized_repair "
            "WHERE commande_uid = :commande_uid"
            );

        const auto &step = *commande.step3;

        stepQuery.bindValue(":reference", step.reference);

        if (step.bottomRetentionFace.has_value())
            stepQuery.bindValue(
                ":bottom_retention_face",
                *step.bottomRetentionFace);
        else
            stepQuery.bindValue(
                ":bottom_retention_face",
                QVariant());

        if (step.bottomRetentionRight.has_value())
            stepQuery.bindValue(
                ":bottom_retention_right",
                *step.bottomRetentionRight);
        else
            stepQuery.bindValue(
                ":bottom_retention_right",
                QVariant());

        if (step.bottomRetentionLeft.has_value())
            stepQuery.bindValue(
                ":bottom_retention_left",
                *step.bottomRetentionLeft);
        else
            stepQuery.bindValue(
                ":bottom_retention_left",
                QVariant());

        if (step.bottomRetentionBehind.has_value())
            stepQuery.bindValue(
                ":bottom_retention_behind",
                *step.bottomRetentionBehind);
        else
            stepQuery.bindValue(
                ":bottom_retention_behind",
                QVariant());

        if (step.upperRetention.has_value())
            stepQuery.bindValue(
                ":upper_retention",
                *step.upperRetention);
        else
            stepQuery.bindValue(
                ":upper_retention",
                QVariant());

        if (step.liftingRings.has_value())
            stepQuery.bindValue(
                ":lifting_rings",
                *step.liftingRings);
        else
            stepQuery.bindValue(
                ":lifting_rings",
                QVariant());

        if (step.forkliftPass.has_value())
            stepQuery.bindValue(
                ":forklift_pass",
                *step.forkliftPass);
        else
            stepQuery.bindValue(
                ":forklift_pass",
                QVariant());

        if (step.dashboard.has_value())
            stepQuery.bindValue(
                ":dashboard",
                *step.dashboard);
        else
            stepQuery.bindValue(
                ":dashboard",
                QVariant());

        stepQuery.bindValue(
            ":unauthorized_repair",
            step.unauthorizedRepair);

        stepQuery.bindValue(":commande_uid", uid);

        if (!stepQuery.exec())
        {
            qCritical()
            << "Erreur SQL modification step3 :"
            << stepQuery.lastError().text();

            db.rollback();
            return UpdateCommandeResult::Error;
        }

        if (stepQuery.numRowsAffected() == 0)
        {
            QSqlQuery insertQuery;

            insertQuery.prepare(
                "INSERT INTO commande_step3 "
                "(commande_uid, reference, bottom_retention_face, "
                "bottom_retention_right, bottom_retention_left, "
                "bottom_retention_behind, upper_retention, "
                "lifting_rings, forklift_pass, dashboard, "
                "unauthorized_repair) "
                "VALUES "
                "(:commande_uid, :reference, "
                ":bottom_retention_face, :bottom_retention_right, "
                ":bottom_retention_left, :bottom_retention_behind, "
                ":upper_retention, :lifting_rings, :forklift_pass, "
                ":dashboard, :unauthorized_repair)"
                );

            insertQuery.bindValue(":commande_uid", uid);
            insertQuery.bindValue(":reference", step.reference);

            if (step.bottomRetentionFace.has_value())
                insertQuery.bindValue(
                    ":bottom_retention_face",
                    *step.bottomRetentionFace);
            else
                insertQuery.bindValue(
                    ":bottom_retention_face",
                    QVariant());

            if (step.bottomRetentionRight.has_value())
                insertQuery.bindValue(
                    ":bottom_retention_right",
                    *step.bottomRetentionRight);
            else
                insertQuery.bindValue(
                    ":bottom_retention_right",
                    QVariant());

            if (step.bottomRetentionLeft.has_value())
                insertQuery.bindValue(
                    ":bottom_retention_left",
                    *step.bottomRetentionLeft);
            else
                insertQuery.bindValue(
                    ":bottom_retention_left",
                    QVariant());

            if (step.bottomRetentionBehind.has_value())
                insertQuery.bindValue(
                    ":bottom_retention_behind",
                    *step.bottomRetentionBehind);
            else
                insertQuery.bindValue(
                    ":bottom_retention_behind",
                    QVariant());

            if (step.upperRetention.has_value())
                insertQuery.bindValue(
                    ":upper_retention",
                    *step.upperRetention);
            else
                insertQuery.bindValue(
                    ":upper_retention",
                    QVariant());

            if (step.liftingRings.has_value())
                insertQuery.bindValue(
                    ":lifting_rings",
                    *step.liftingRings);
            else
                insertQuery.bindValue(
                    ":lifting_rings",
                    QVariant());

            if (step.forkliftPass.has_value())
                insertQuery.bindValue(
                    ":forklift_pass",
                    *step.forkliftPass);
            else
                insertQuery.bindValue(
                    ":forklift_pass",
                    QVariant());

            if (step.dashboard.has_value())
                insertQuery.bindValue(
                    ":dashboard",
                    *step.dashboard);
            else
                insertQuery.bindValue(
                    ":dashboard",
                    QVariant());

            insertQuery.bindValue(
                ":unauthorized_repair",
                step.unauthorizedRepair);

            if (!insertQuery.exec())
            {
                qCritical()
                << "Erreur SQL insertion step3 :"
                << insertQuery.lastError().text();

                db.rollback();
                return UpdateCommandeResult::Error;
            }
        }
    }

    if (commande.step4.has_value())
    {
        QSqlQuery stepQuery;

        stepQuery.prepare(
            "UPDATE commande_step4 "
            "SET reference = :reference, "
            "internal_na = :internal_na, "
            "internal_ok = :internal_ok, "
            "internal_clean = :internal_clean, "
            "internal_object_inside = :internal_object_inside, "
            "internal_pollution = :internal_pollution "
            "WHERE commande_uid = :commande_uid"
            );

        const auto &step = *commande.step4;

        stepQuery.bindValue(":reference", step.reference);
        stepQuery.bindValue(":internal_na", step.internalNA);

        if (step.internalOK.has_value())
            stepQuery.bindValue(":internal_ok", *step.internalOK);
        else
            stepQuery.bindValue(":internal_ok", QVariant());

        stepQuery.bindValue(":internal_clean", step.internalClean);
        stepQuery.bindValue(
            ":internal_object_inside",
            step.internalObjectInside);
        stepQuery.bindValue(
            ":internal_pollution",
            step.internalPollution);

        stepQuery.bindValue(":commande_uid", uid);

        if (!stepQuery.exec())
        {
            qCritical()
            << "Erreur SQL modification step4 :"
            << stepQuery.lastError().text();

            db.rollback();
            return UpdateCommandeResult::Error;
        }

        if (stepQuery.numRowsAffected() == 0)
        {
            QSqlQuery insertQuery;

            insertQuery.prepare(
                "INSERT INTO commande_step4 "
                "(commande_uid, reference, internal_na, internal_ok, "
                "internal_clean, internal_object_inside, "
                "internal_pollution) "
                "VALUES "
                "(:commande_uid, :reference, :internal_na, :internal_ok, "
                ":internal_clean, :internal_object_inside, "
                ":internal_pollution)"
                );

            insertQuery.bindValue(":commande_uid", uid);
            insertQuery.bindValue(":reference", step.reference);
            insertQuery.bindValue(":internal_na", step.internalNA);

            if (step.internalOK.has_value())
                insertQuery.bindValue(
                    ":internal_ok",
                    *step.internalOK);
            else
                insertQuery.bindValue(
                    ":internal_ok",
                    QVariant());

            insertQuery.bindValue(
                ":internal_clean",
                step.internalClean);
            insertQuery.bindValue(
                ":internal_object_inside",
                step.internalObjectInside);
            insertQuery.bindValue(
                ":internal_pollution",
                step.internalPollution);

            if (!insertQuery.exec())
            {
                qCritical()
                << "Erreur SQL insertion step4 :"
                << insertQuery.lastError().text();

                db.rollback();
                return UpdateCommandeResult::Error;
            }
        }
    }

    if (commande.step5.has_value())
    {
        QSqlQuery stepQuery;

        stepQuery.prepare(
            "UPDATE commande_step5 "
            "SET reference = :reference, "
            "epaisseur_na = :epaisseur_na, "
            "epaisseur_min_side_front = :epaisseur_min_side_front, "
            "epaisseur_min_side_back = :epaisseur_min_side_back, "
            "epaisseur_min_side_right = :epaisseur_min_side_right, "
            "epaisseur_min_side_left = :epaisseur_min_side_left, "

            "epaisseur_side_front_result1 = :epaisseur_side_front_result1, "
            "epaisseur_side_front_result2 = :epaisseur_side_front_result2, "
            "epaisseur_side_front_result3 = :epaisseur_side_front_result3, "
            "epaisseur_side_front_result4 = :epaisseur_side_front_result4, "
            "epaisseur_side_front_result5 = :epaisseur_side_front_result5, "

            "epaisseur_side_back_result1 = :epaisseur_side_back_result1, "
            "epaisseur_side_back_result2 = :epaisseur_side_back_result2, "
            "epaisseur_side_back_result3 = :epaisseur_side_back_result3, "
            "epaisseur_side_back_result4 = :epaisseur_side_back_result4, "
            "epaisseur_side_back_result5 = :epaisseur_side_back_result5, "

            "epaisseur_side_right_result1 = :epaisseur_side_right_result1, "
            "epaisseur_side_right_result2 = :epaisseur_side_right_result2, "
            "epaisseur_side_right_result3 = :epaisseur_side_right_result3, "
            "epaisseur_side_right_result4 = :epaisseur_side_right_result4, "
            "epaisseur_side_right_result5 = :epaisseur_side_right_result5, "

            "epaisseur_side_left_result1 = :epaisseur_side_left_result1, "
            "epaisseur_side_left_result2 = :epaisseur_side_left_result2, "
            "epaisseur_side_left_result3 = :epaisseur_side_left_result3, "
            "epaisseur_side_left_result4 = :epaisseur_side_left_result4, "
            "epaisseur_side_left_result5 = :epaisseur_side_left_result5 "

            "WHERE commande_uid = :commande_uid"
            );

        const auto &step = *commande.step5;

        stepQuery.bindValue(":reference", step.reference);
        stepQuery.bindValue(":epaisseur_na", step.epaisseurNA);

        stepQuery.bindValue(
            ":epaisseur_min_side_front",
            step.epaisseurMinSideFront);
        stepQuery.bindValue(
            ":epaisseur_min_side_back",
            step.epaisseurMinSideBack);
        stepQuery.bindValue(
            ":epaisseur_min_side_right",
            step.epaisseurMinSideRight);
        stepQuery.bindValue(
            ":epaisseur_min_side_left",
            step.epaisseurMinSideLeft);

        stepQuery.bindValue(
            ":epaisseur_side_front_result1",
            step.epaisseurSideFrontResult1);
        stepQuery.bindValue(
            ":epaisseur_side_front_result2",
            step.epaisseurSideFrontResult2);
        stepQuery.bindValue(
            ":epaisseur_side_front_result3",
            step.epaisseurSideFrontResult3);
        stepQuery.bindValue(
            ":epaisseur_side_front_result4",
            step.epaisseurSideFrontResult4);
        stepQuery.bindValue(
            ":epaisseur_side_front_result5",
            step.epaisseurSideFrontResult5);

        stepQuery.bindValue(
            ":epaisseur_side_back_result1",
            step.epaisseurSideBackResult1);
        stepQuery.bindValue(
            ":epaisseur_side_back_result2",
            step.epaisseurSideBackResult2);
        stepQuery.bindValue(
            ":epaisseur_side_back_result3",
            step.epaisseurSideBackResult3);
        stepQuery.bindValue(
            ":epaisseur_side_back_result4",
            step.epaisseurSideBackResult4);
        stepQuery.bindValue(
            ":epaisseur_side_back_result5",
            step.epaisseurSideBackResult5);

        stepQuery.bindValue(
            ":epaisseur_side_right_result1",
            step.epaisseurSideRightResult1);
        stepQuery.bindValue(
            ":epaisseur_side_right_result2",
            step.epaisseurSideRightResult2);
        stepQuery.bindValue(
            ":epaisseur_side_right_result3",
            step.epaisseurSideRightResult3);
        stepQuery.bindValue(
            ":epaisseur_side_right_result4",
            step.epaisseurSideRightResult4);
        stepQuery.bindValue(
            ":epaisseur_side_right_result5",
            step.epaisseurSideRightResult5);

        stepQuery.bindValue(
            ":epaisseur_side_left_result1",
            step.epaisseurSideLeftResult1);
        stepQuery.bindValue(
            ":epaisseur_side_left_result2",
            step.epaisseurSideLeftResult2);
        stepQuery.bindValue(
            ":epaisseur_side_left_result3",
            step.epaisseurSideLeftResult3);
        stepQuery.bindValue(
            ":epaisseur_side_left_result4",
            step.epaisseurSideLeftResult4);
        stepQuery.bindValue(
            ":epaisseur_side_left_result5",
            step.epaisseurSideLeftResult5);

        stepQuery.bindValue(":commande_uid", uid);

        if (!stepQuery.exec())
        {
            qCritical()
            << "Erreur SQL modification step5 :"
            << stepQuery.lastError().text();

            db.rollback();
            return UpdateCommandeResult::Error;
        }

        if (stepQuery.numRowsAffected() == 0)
        {
            QSqlQuery insertQuery;

            insertQuery.prepare(
                "INSERT INTO commande_step5 ("
                "commande_uid, reference, epaisseur_na, "
                "epaisseur_min_side_front, epaisseur_min_side_back, "
                "epaisseur_min_side_right, epaisseur_min_side_left, "

                "epaisseur_side_front_result1, epaisseur_side_front_result2, "
                "epaisseur_side_front_result3, epaisseur_side_front_result4, "
                "epaisseur_side_front_result5, "

                "epaisseur_side_back_result1, epaisseur_side_back_result2, "
                "epaisseur_side_back_result3, epaisseur_side_back_result4, "
                "epaisseur_side_back_result5, "

                "epaisseur_side_right_result1, epaisseur_side_right_result2, "
                "epaisseur_side_right_result3, epaisseur_side_right_result4, "
                "epaisseur_side_right_result5, "

                "epaisseur_side_left_result1, epaisseur_side_left_result2, "
                "epaisseur_side_left_result3, epaisseur_side_left_result4, "
                "epaisseur_side_left_result5"
                ") VALUES ("
                ":commande_uid, :reference, :epaisseur_na, "
                ":epaisseur_min_side_front, :epaisseur_min_side_back, "
                ":epaisseur_min_side_right, :epaisseur_min_side_left, "

                ":epaisseur_side_front_result1, :epaisseur_side_front_result2, "
                ":epaisseur_side_front_result3, :epaisseur_side_front_result4, "
                ":epaisseur_side_front_result5, "

                ":epaisseur_side_back_result1, :epaisseur_side_back_result2, "
                ":epaisseur_side_back_result3, :epaisseur_side_back_result4, "
                ":epaisseur_side_back_result5, "

                ":epaisseur_side_right_result1, :epaisseur_side_right_result2, "
                ":epaisseur_side_right_result3, :epaisseur_side_right_result4, "
                ":epaisseur_side_right_result5, "

                ":epaisseur_side_left_result1, :epaisseur_side_left_result2, "
                ":epaisseur_side_left_result3, :epaisseur_side_left_result4, "
                ":epaisseur_side_left_result5"
                ")"
                );

            insertQuery.bindValue(":commande_uid", uid);
            insertQuery.bindValue(":reference", step.reference);
            insertQuery.bindValue(":epaisseur_na", step.epaisseurNA);

            insertQuery.bindValue(
                ":epaisseur_min_side_front",
                step.epaisseurMinSideFront);
            insertQuery.bindValue(
                ":epaisseur_min_side_back",
                step.epaisseurMinSideBack);
            insertQuery.bindValue(
                ":epaisseur_min_side_right",
                step.epaisseurMinSideRight);
            insertQuery.bindValue(
                ":epaisseur_min_side_left",
                step.epaisseurMinSideLeft);

            insertQuery.bindValue(
                ":epaisseur_side_front_result1",
                step.epaisseurSideFrontResult1);
            insertQuery.bindValue(
                ":epaisseur_side_front_result2",
                step.epaisseurSideFrontResult2);
            insertQuery.bindValue(
                ":epaisseur_side_front_result3",
                step.epaisseurSideFrontResult3);
            insertQuery.bindValue(
                ":epaisseur_side_front_result4",
                step.epaisseurSideFrontResult4);
            insertQuery.bindValue(
                ":epaisseur_side_front_result5",
                step.epaisseurSideFrontResult5);

            insertQuery.bindValue(
                ":epaisseur_side_back_result1",
                step.epaisseurSideBackResult1);
            insertQuery.bindValue(
                ":epaisseur_side_back_result2",
                step.epaisseurSideBackResult2);
            insertQuery.bindValue(
                ":epaisseur_side_back_result3",
                step.epaisseurSideBackResult3);
            insertQuery.bindValue(
                ":epaisseur_side_back_result4",
                step.epaisseurSideBackResult4);
            insertQuery.bindValue(
                ":epaisseur_side_back_result5",
                step.epaisseurSideBackResult5);

            insertQuery.bindValue(
                ":epaisseur_side_right_result1",
                step.epaisseurSideRightResult1);
            insertQuery.bindValue(
                ":epaisseur_side_right_result2",
                step.epaisseurSideRightResult2);
            insertQuery.bindValue(
                ":epaisseur_side_right_result3",
                step.epaisseurSideRightResult3);
            insertQuery.bindValue(
                ":epaisseur_side_right_result4",
                step.epaisseurSideRightResult4);
            insertQuery.bindValue(
                ":epaisseur_side_right_result5",
                step.epaisseurSideRightResult5);

            insertQuery.bindValue(
                ":epaisseur_side_left_result1",
                step.epaisseurSideLeftResult1);
            insertQuery.bindValue(
                ":epaisseur_side_left_result2",
                step.epaisseurSideLeftResult2);
            insertQuery.bindValue(
                ":epaisseur_side_left_result3",
                step.epaisseurSideLeftResult3);
            insertQuery.bindValue(
                ":epaisseur_side_left_result4",
                step.epaisseurSideLeftResult4);
            insertQuery.bindValue(
                ":epaisseur_side_left_result5",
                step.epaisseurSideLeftResult5);

            if (!insertQuery.exec())
            {
                qCritical()
                << "Erreur SQL insertion step5 :"
                << insertQuery.lastError().text();

                db.rollback();
                return UpdateCommandeResult::Error;
            }
        }
    }

    if (commande.step6.has_value())
    {
        QSqlQuery stepQuery;

        stepQuery.prepare(
            "UPDATE commande_step6 "
            "SET reference = :reference, "
            "etancheite_ok1 = :etancheite_ok1, "
            "etancheite_date1 = :etancheite_date1, "
            "etancheite_bar1 = :etancheite_bar1, "
            "etancheite_ok2 = :etancheite_ok2, "
            "etancheite_date2 = :etancheite_date2, "
            "etancheite_bar2 = :etancheite_bar2 "
            "WHERE commande_uid = :commande_uid"
            );

        const auto &step = *commande.step6;

        stepQuery.bindValue(":reference", step.reference);

        if (step.etancheiteOK1.has_value())
            stepQuery.bindValue(
                ":etancheite_ok1",
                *step.etancheiteOK1);
        else
            stepQuery.bindValue(
                ":etancheite_ok1",
                QVariant());

        stepQuery.bindValue(
            ":etancheite_date1",
            step.etancheiteDate1);

        stepQuery.bindValue(
            ":etancheite_bar1",
            step.etancheiteBar1);

        if (step.etancheiteOK2.has_value())
            stepQuery.bindValue(
                ":etancheite_ok2",
                *step.etancheiteOK2);
        else
            stepQuery.bindValue(
                ":etancheite_ok2",
                QVariant());

        stepQuery.bindValue(
            ":etancheite_date2",
            step.etancheiteDate2);

        stepQuery.bindValue(
            ":etancheite_bar2",
            step.etancheiteBar2);

        stepQuery.bindValue(":commande_uid", uid);

        if (!stepQuery.exec())
        {
            qCritical()
            << "Erreur SQL modification step6 :"
            << stepQuery.lastError().text();

            db.rollback();
            return UpdateCommandeResult::Error;
        }

        if (stepQuery.numRowsAffected() == 0)
        {
            QSqlQuery insertQuery;

            insertQuery.prepare(
                "INSERT INTO commande_step6 "
                "(commande_uid, reference, etancheite_ok1, "
                "etancheite_date1, etancheite_bar1, "
                "etancheite_ok2, etancheite_date2, etancheite_bar2) "
                "VALUES "
                "(:commande_uid, :reference, :etancheite_ok1, "
                ":etancheite_date1, :etancheite_bar1, "
                ":etancheite_ok2, :etancheite_date2, :etancheite_bar2)"
                );

            insertQuery.bindValue(":commande_uid", uid);
            insertQuery.bindValue(":reference", step.reference);

            if (step.etancheiteOK1.has_value())
                insertQuery.bindValue(
                    ":etancheite_ok1",
                    *step.etancheiteOK1);
            else
                insertQuery.bindValue(
                    ":etancheite_ok1",
                    QVariant());

            insertQuery.bindValue(
                ":etancheite_date1",
                step.etancheiteDate1);

            insertQuery.bindValue(
                ":etancheite_bar1",
                step.etancheiteBar1);

            if (step.etancheiteOK2.has_value())
                insertQuery.bindValue(
                    ":etancheite_ok2",
                    *step.etancheiteOK2);
            else
                insertQuery.bindValue(
                    ":etancheite_ok2",
                    QVariant());

            insertQuery.bindValue(
                ":etancheite_date2",
                step.etancheiteDate2);

            insertQuery.bindValue(
                ":etancheite_bar2",
                step.etancheiteBar2);

            if (!insertQuery.exec())
            {
                qCritical()
                << "Erreur SQL insertion step6 :"
                << insertQuery.lastError().text();

                db.rollback();
                return UpdateCommandeResult::Error;
            }
        }
    }

    if (!db.commit())
    {
        qCritical()
        << "Erreur lors du commit de la commande :"
        << db.lastError().text();

        db.rollback();

        return UpdateCommandeResult::Error;
    }


    qInfo()
        << "Commande modifiée avec uid :"
        << uid;

    return UpdateCommandeResult::Success;
}