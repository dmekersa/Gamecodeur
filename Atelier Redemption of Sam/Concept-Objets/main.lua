io.stdout:setvbuf('no')
if arg[#arg] == "-debug" then require("mobdebug").start() end

-- Le héros !
local samSprite = {}

local boss = {}

function CreeSprite(pNom)
  print("Crée un sprite")
  local sprite = {}
  sprite.nom = pNom
  
  sprite.ChangeNom = function(pNouveauNom)
    sprite.nom = pNouveauNom
  end
  
  return sprite
end

samSprite = CreeSprite("Sam")
print("Nom de Sam",samSprite.nom)

boss = CreeSprite("TheBoss")
print("Nom du boss",boss.nom)

boss.ChangeNom("The New Boss !")
samSprite.ChangeNom("New Sam")
print("Nom du boss",boss.nom)
print("Nom de Sam",samSprite.nom)
