-- Module simple, permet simplement de thématiser son code
require("module1")
AjouteAlien()

-- Approche avancé avec self, on ne peut pas créer plusieurs instances
local Aliens2 = require("module2")
Aliens2:AjouteAlien()
-- La prochaine ligne ne crée pas de nouvelle instance mais récupère la précédente
-- Du coup, Aliens2 est égal à Aliens3
local Aliens2bis = require("module2")
Aliens2bis:AjouteAlien()

-- Approche mixte, on peut "simuler" la création de plusieurs instances
require("module3")
local Aliens3 = CreeAliens()
Aliens3.AjouteAlien()
local Aliens3bis = CreeAliens()
Aliens3bis.AjouteAlien()
Aliens3bis.AjouteAlien()

-- Approche Objet, on peut créer plusieurs instances
local Aliens4 = require("module4")

local mesAliens1 = Aliens4.nouveau()

mesAliens1:AjouteAlien()

local mesAliens2 = Aliens4.nouveau()

mesAliens2:AjouteAlien()
mesAliens2:AjouteAlien()