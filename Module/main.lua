-- Module simple, permet simplement de thématiser son code
require("module1")
AjouteAlien()

-- Module avancé avec self, on ne peut pas créer plusieurs instances
local Aliens2 = require("module2")
Aliens2:AjouteAlien()
local Aliens3 = require("module2")
Aliens3:AjouteAlien()

-- Module mixte, on peut simuler la création de plusieurs instances
require("module3")
local Aliens3 = CreeAlien()
Aliens3.AjouteAlien()
local Aliens3bis = CreeAlien()
Aliens3bis.AjouteAlien()
Aliens3bis.AjouteAlien()

-- Objet, on peut créer plusieurs instances
local Aliens4 = require("module4")

local mesAliens1 = Aliens4.nouveau()

mesAliens1:AjouteAlien()

local mesAliens2 = Aliens4.nouveau()

mesAliens2:AjouteAlien()
mesAliens2:AjouteAlien()
