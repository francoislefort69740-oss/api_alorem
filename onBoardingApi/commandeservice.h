#pragma once

#include "CommandeRepository.h"


enum class UpdateCommandeServiceResult
{
    Success,
    NotFound,
    Error
};

class CommandeService
{
public:
    explicit CommandeService(CommandeRepository &repository);

    QList<Commande> getAllCommandes();

    std::optional<CommandeComplete> getCommandeById(int uid);

    bool validateCommandeComplete(const CommandeComplete &commande);

    bool createCommande(const CommandeComplete &commande);

    UpdateCommandeServiceResult updateCommande(int uid, const CommandeComplete &commande);

private:
    CommandeRepository &repository;
};