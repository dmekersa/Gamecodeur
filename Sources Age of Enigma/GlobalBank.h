/*
 *  GlobalBank.h
 *  enigma
 *
 *  Created by Rockford on 06/05/10.
 *  Copyright 2010 Casual Games France. All rights reserved.
 *
 */


#ifndef GLOBALBANK_H
#define GLOBALBANK_H

#include "KPTK.h"

class EGlobalBank : public KObjectListable {
public:
   char _szName[256];
   char _szValue[1024*4];
   bool _bTask;
   
   // Liste
	static KList<EGlobalBank> g_ListGlobal;
   
   // Demande une valeur texte à la banque
   static const char* getStrValue(const char *szName, const char *szDefault);
   // Demande une valeur entière à la banque
   static int getIntValue(const char *szName, int nDefault);
   // Ajoute une valeur à la banque
   static void AddStrValue(const char *szName, const char *szValue, bool _b = false);
   // Ajoute une tâche d'aventure (non résolue)
   static void AddTaskUnique(const char *szName);
   // Ajoute une tâche d'aventure et ajoute une séquence "New Objective"
   static void AddNewObjective(const char *szUniverse, const char *szName);
   // Vérifie si un objectif est résolu (valeur == 1)
   static bool getObjectiveResolved(const char *szUniverse, const char *szName);
   // Ajoute un hint lié à un objectif
   static void AddNewHint(const char *szUniverse, const char *szObjective, const char *szName);
   // Supprime un hint lié à un objectif
   static void RemoveOneHint(const char *szUniverse, const char *szObjective, const char *szName);
   // Supprime une globale
   static bool RemoveValue(const char *szName);
   // Supprime les globales commençant pas une chaine
   static void RemoveValueStartWith(const char *szStartWith);
   // Résoud une tâche
   static void ResolveOneTask(const char *szName);
   // Résoud une tâche (objectif) et supprime les hints associés
   static void ResolveTaskHints(const char *szUniverse, const char *szObjective);
   // Positionne l'entrée à 1
   static void SetTo1(const char *szName);
   // Change une valeur dans la banque
   static void ChangeStrValue(const char *szName, const char *szValue);
   // Retourne si la valeur existe
   static bool isExist(const char *szName);
   // Affiche grossièrement la liste des todo task
   static void ShowTaskList(float y, const char *szUniverseName);
   
   // Destructeur de chaque élement
   static void CleanUp(){
      // Parcours la liste pour en récupérer les valeurs et la vide
      EGlobalBank *o;
      EGlobalBank *otodelete;
      bool bGotNext = g_ListGlobal.getHead (o);
      if (bGotNext) {
         while (bGotNext) {
            otodelete = o;
            g_ListGlobal.remove(o);
            bGotNext = g_ListGlobal.getNext (o);
            delete otodelete;
         }
      }
   }
};

#define AddObjective(__szUniverse,__szObjective) SCENEDIRECTOR->getSequencer()->Objective(NULL, __szUniverse,__szObjective)
#define AddTask(__sz) EGlobalBank::AddTaskUnique(__sz)
#define ResetTask(__sz) EGlobalBank::AddStrValue(__sz, "0", true)
#define AddHint(__szUniverse,__szObjective,__szQuestion) EGlobalBank::AddNewHint(__szUniverse,__szObjective,__szQuestion)
#define RemoveHint(__szUniverse,__szObjective,__szQuestion) EGlobalBank::RemoveOneHint(__szUniverse,__szObjective,__szQuestion)
#define ResolveTask(__sz) EGlobalBank::ResolveOneTask(__sz)
#define ResolveObjective(__szUniverse,__szObjective) EGlobalBank::ResolveTaskHints(__szUniverse,__szObjective)
#define TaskResolved(__sz) (EGlobalBank::getIntValue(__sz,0)==1)
#define ObjectiveResolved(__szUniverse, __szName) (EGlobalBank::getObjectiveResolved(__szUniverse, __szName))
#define TaskToResolve(__sz) (EGlobalBank::getIntValue(__sz,-1)==0)
#define SetGlobal(__sz,__szval) EGlobalBank::AddStrValue(__sz,__szval)
#define GetGlobal(__sz,__ndef) EGlobalBank::getIntValue(__sz,__ndef)
#define GetStrGlobal(__sz,__szdef) EGlobalBank::getStrValue(__sz,__szdef)
#define TestGlobal(__sz) EGlobalBank::getIntValue(__sz,0)==1
#define TestStrGlobal(__sz) (strlen(EGlobalBank::getStrValue(__sz,""))>0)

#endif

// Liste des tâches
// objective_house_enterhouse               Entrer dans la maison
// task_house_hall_light               Allumer
// task_house_hall_repair_switch       Réparer le switch dans le hall
// task_house_hall_escape_nathan       Nathan s'est enfui
// task_house_hall_groundfloor         Ground floor résolu, on peut maintenant monter
// task_house_firstfloor__isaacreunion  Retrouvailles avec Isaac
// task_house_dining_contactghost      Contact avec Murray
// task_house_dining_putframe          Mettre le cadre au mur
// task_house_dining_putvase           Mettre le vase sur le buffet
// task_house_dining_stopfire          Eteindre le feu du tapis
// task_house_dining_openrightwindow   Ouvrir la vitrine de droite
// task_house_dining_openbook          Ouvrir le livre dans la bibliothèque
// task_house_dining_repairboat        Réparer le bateau
// task_house_dining_openboat          Ouvrir le bateau
// task_house_dining_firstdrawer       Ouvrir le 1er tiroir
// task_house_dining_seconddrawer      Ouvrir le 1er tiroir
// task_house_dining_cleanpicture      Nettoyer la photo
// task_house_dining_givetreasure      Donne le trésor à Murray
// task_house_kitchen_killwatermarc    Chasser le poltergeist de la cuisine
// task_house_dining_gotouniverse      Aller à l'univers immédiatement
// task_house_kitchen_fill_pan         Remplir la casserole
// task_house_room2_contactghost       Contacter le fantôme dans la chambre 2
// task_house_room2_openbed            Ouvrir le lit
// task_house_room2_openbedside        Ouvrir le tiroir de la table de nuit
// task_house_room2_repairmirror       Réparer le miroir
// task_house_room2_getbible           Trouver la bible
// task_house_room2_givebible          Donner la bible au prêtre
// task_house_room2_expelevil          Chasser le mal du miroir
// task_house_room2_fragment00-08      Repositionner les fragments du miroir
// task_house_room2_freepoltergeist    Libérer le poltergeist du miroir
// task_house_room2_killpoltergeist    Tuer le poltergeist

// ISLAND
// task_island_expelparrot       Chasser le perroquet de la crique
// task_island_removebranches    Enlever les branchages
// task_island_expelshark        Chasser le requin
// task_island_fallpearl         Faire tomber la perle
// task_island_getpearl          Prendre la perle
// task_island_clamwood          Mettre le baton dans le bénitier
// task_island_getkey            Prendre la clé dans le bénitier
// task_island_combinemedallion  Combiner le médaillon
// task_island_opensafe          Prendre la pierre de sagesse
// task_island_neckless          Assembler le 2ème morceau de pendentif
// task_island_getstone          Prendre la pierre de sagesse
// task_island_revealqueen			Faire apparaitre la reine

// task_island_prologcompleted   Prologue complété
// task_island_universeresolved  ************* Univers Island résolu *************

// ****************************************
// * JAPON                                *
// ****************************************

// task_house_basement_opendoor        ouvre la porte de la cave
// task_house_basement_psycho          1er dialogue qui fait sursauter
// task_house_basement_removebulb      bulbe cassé enlevé avec le torchon
// task_house_basement_changebulb      ampoule changée
// task_house_basement_light           lumière allumée
// task_house_basement_opengrid        grille au sol ouverte
// task_house_basement_reversecoal     renverser le charbon
// task_house_basement_brokevase       casser le vase
// task_house_basement_opentoolbox     ouvrir la boite à outils
// task_house_basement_givepaper       donner le papier au fantome
// task_house_basement_givecharcoal    donner le fusain au fantome
// task_house_basement_bonzai          tailler le bonzai
// task_house_basement_getrope         Obtenir la corde de Natsumi
// task_japan_tidy					Ranger la chambre
// task_japan_garden             refaire le jardin de sable
// task_japan_lamp1 / 2 / 3      allumer la lampe sur le chemin
// task_japan_revealbank         active le mini jeu du banc
// task_japan_resolvebank        réussir le mini jeu sur le banc
// task_japan_screen             ranger le paravent
// task_japan_bed                faire le lit
// task_japan_stones             les galets dans la chambre à remettre en ordre
// task_japan_torch              la torche au mur
// task_japan_table              recomposer et poser la table
// task_japan_teapot             décrocher et poser la théière
// task_japan_underteapot        ramasser et poser le sous théière
// task_japan_bonzai             replacer le bonzai
// task_japan_candlelamp         replacer la lampe au dessus du lit
// task_japan_candlelamplight    allumer la lampe au dessus du lit
// task_japan_encens             recomposer et pose le porte encens
// task_japan_encenslight        allume l'encens
// task_japan_bento              recomposer et poser le bento
// task_japan_veil               animer le voile
// task_japan_leaves             Poser les pétales sur le lit
// task_universe_done_japan      ************ Univers Japon résolu *************
// task_freeghost_done_japan


// ****************************************
// * INCA                                 *
// ****************************************
// task_house_livingroom_repairstatuette	Réparer la statuette
// task_house_livingroom_placebook		Placer le livre
// task_house_livingroom_lefteye       Mettre l'oeil gauche à la statuette
// task_house_livingroom_contactghost  Contact avec le guerrier Inca
// task_house_livingroom_memorychess   Trouver la piece   
// task_house_livingroom_getropeladder Obtenir l'échelle de corde
// task_house_livingroom_killpoltergeist Tuer le poltereist
// task_house_livingroom_playpiano  Jouer les notes au piano
// task_house_kapangya4effets    Activer les 4 effets sur la statuette
// task_house_kapangya_effect1   Activer l'effet 1 (elevation)
// task_house_kapangya_effect2   Activer l'effet 2 (illumination)
// task_house_kapangya_effect3   Activer l'effet 3 (tremblement)
// task_house_kapangya_effect4   Activer l'effet 4 (eclipse)
// task_house_useropeladder      Utiliser l'échelle pour monter au 1er
// task_inca_opencube            Ouvrir le bloc de pierre sur l'écran principal
// task_inca_findspear           Trouver les morceaux de lance
// task_inca_givespear           Donner la lance
// task_inca_opendoor            Ouvrir la porte du temple
// task_inca_getartefacts        Trouver les 4 artefacts
// task_inca_entertemple         Entrer dans le temple
// task_inca_stairslight         Illuminer les escaliers
// task_inca_findfresco1 et 2    Trouver les morceaux manquants de la fresque
// task_inca_repairfresco1 et 2  Réparer la fresque
// task_inca_fillhole            Remplir le trou pour traverser le couloir
// task_inca_godsign             Provoquer un signe des dieux
// task_inca_sacrificekapangya   Sacrifier la statuette
// task_inca_placekapangya       Poser la statuette sur l'autel
// task_inca_placeartefact1      Placer l'artefact sur l'autel
// task_inca_placeartefact2      Placer l'artefact sur l'autel      
// task_inca_placeartefact3      Placer l'artefact sur l'autel
// task_inca_placeartefact4      Placer l'artefact sur l'autel
// task_universe_done_inca
// task_freeghost_done_inca

// ****************************************
// * MOYEN AGE                            *
// ****************************************
// task_middleage_getcork        Obtenir du liège
// task_middleage_gather_dill
// task_middleage_gather_garlic
// task_middleage_gather_ginkgo
// task_middleage_gather_fennel
// task_middleage_gather_poppy
// task_middleage_gather_rue
// task_middleage_gather_wort
// task_middleage_gather_chervil
// task_middleage_contactwitch   Contacter la sorcière
// task_middleage_getplate       Procurer une assiette à la sorcière
// task_middleage_putplate       Poser l'assiette au sol
// task_middleage_getplant1 à 6	Apporter la 1ère plante
// task_middleage_putplant1 à 6  Poser la plante dans l'assiette
// task_middleage_startprepare   Début de la préparation
// task_middleage_prepareplant1 à 6 Préparer la plante dans l'assiette avec le pilon
// task_middleage_givecontent     Procurer la préparation contenue dans l'assiette
// task_middleage_giveflask       Procurer une fiole à la sorcière
// task_middleage_treatabbot     Soigner l'abbé
// task_universe_done_middleage
// task_freeghost_done_middleage

// ****************************************
// * EGYPTE                               *
// ****************************************
// task_house_bathroom_contactghost		Rentrer en contact avec le fantôme
// task_house_bathroom_opencurtain		Ouvrir le rideau de douche
// task_house_bathroom_disposeflacon   Disposer les flacons conformément au schéma sur le parchemin
// task_house_bathroom_moveflacon      Déplacer les flacons via le mini jeu blokoban
// task_egypt_talkshop				Parler pour la 1ère fois à la vendeuse
// task_egypt_getkey					Obtenir la clé de la vendeuse
// task_egypt_missingkey			Mettre la clé dans son emplacement
// task_egypt_openmaze           Ouvrir le labyrinthe
// task_egypt_resolveboat        Compléter la barque
// task_egypt_opentomb				Ouvrir la salle du tombeau
// task_egypt_meetheres				Rencontrer HERES, la mère de NEFERES
// task_egypt_purifying				Réactiver le mécanisme de purification
// task_egypt_ordergods				Mettre les dieux dans l'ordre
// task_universe_done_egypt
// task_freeghost_done_egypt

// ****************************************
// * ENFER                                *
// ****************************************
// task_house_openatticdoor		Ouvrir la porte du grenier
// task_house_drawatticdoor      Dessiner sur la porte du grenier
// task_house_activeatticdoor    Activer le pentagramme sur la porte du grenier
// task_house_attic_meetgirl		Rencontre avec la fillette
// task_house_attic_untangle     Déméler les bougies
// task_house_attic_resolvepentacle Mettre les pierres dans le pentacle
// task_hell_foundrichard        Trouver Richard, le frêre de Seth
// task_hell_reassureseth        Rassurer Seth : Richard va bien !
// task_hell_savesoul_1          Donner à la 1ère âme les objets qu'elle attend
// task_hell_savesoul_2          Donner à la 2ème âme les objets qu'elle attend
// task_hell_savesoul_3          Donner à la 3ème âme les objets qu'elle attend
// task_hell_leave					Utiliser la pièce pour traverser le styx
