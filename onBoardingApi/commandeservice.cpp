#include "CommandeService.h"
#include <QDebug>

CommandeService::CommandeService(CommandeRepository &repository)
    : repository(repository)
{
}

QList<Commande> CommandeService::getAllCommandes()
{
    return repository.findAll();
}

std::optional<CommandeComplete>
CommandeService::getCommandeById(int uid)
{
    return repository.findById(uid);
}

bool CommandeService::createCommande(const CommandeComplete &commande)
{
    if (!validateCommandeComplete(commande))
    {
        return false;
    }

    const auto uid = repository.createCommande(commande);

    if (!uid.has_value())
    {
        return false;
    }

    qInfo() << "Commande créée avec uid :"
            << *uid;

    return true;
}

bool CommandeService::validateCommandeComplete(const CommandeComplete &commande)
{
    // Le serialNumber est obligatoire
    if (!commande.commande.serialNumber.has_value())
    {
        qWarning() << "Commande invalide : serialNumber absent";
        return false;
    }

    const int serialNumber = *commande.commande.serialNumber;

    // Vérification de la référence de chaque step présent
    if (commande.step0.has_value() && commande.step0->reference != serialNumber)
    {
        qWarning() << "Commande invalide :"
                   << "reference du step 0 différente du serialNumber";
        return false;
    }

    if (commande.step1.has_value() && commande.step1->reference != serialNumber)
    {
        qWarning() << "Commande invalide :"
                   << "reference du step 1 différente du serialNumber";
        return false;
    }

    if (commande.step2.has_value() && commande.step2->reference != serialNumber)
    {
        qWarning() << "Commande invalide :"
                   << "reference du step 2 différente du serialNumber";
        return false;
    }

    if (commande.step3.has_value() && commande.step3->reference != serialNumber)
    {
        qWarning() << "Commande invalide :"
                   << "reference du step 3 différente du serialNumber";
        return false;
    }

    if (commande.step4.has_value() && commande.step4->reference != serialNumber)
    {
        qWarning() << "Commande invalide :"
                   << "reference du step 4 différente du serialNumber";
        return false;
    }

    if (commande.step5.has_value() && commande.step5->reference != serialNumber)
    {
        qWarning() << "Commande invalide :"
                   << "reference du step 5 différente du serialNumber";
        return false;
    }

    if (commande.step6.has_value() && commande.step6->reference != serialNumber)
    {
        qWarning() << "Commande invalide :"
                   << "reference du step 6 différente du serialNumber";
        return false;
    }

    return true;
}

UpdateCommandeServiceResult CommandeService::updateCommande(int uid, const CommandeComplete &commande)
{
    if (!validateCommandeComplete(commande))
        return UpdateCommandeServiceResult::Error;

    const auto result = repository.updateCommande(uid, commande);

    if (result == UpdateCommandeResult::NotFound)
        return UpdateCommandeServiceResult::NotFound;

    if (result == UpdateCommandeResult::Error)
        return UpdateCommandeServiceResult::Error;

    return UpdateCommandeServiceResult::Success;
}