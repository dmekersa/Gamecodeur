local mobManager = {}

local gameState = require("gamestate")

local imgMouche = love.graphics.newImage("Images/mob1.png")

mobManager.liste_mobs = {}

mobManager.Reset = function()
  mobManager.liste_mobs = {}
end

mobManager.CreeMob = function(pType, pX, pY, pVx, pVy)
   
  local mob = {}
  mob.type = pType
  mob.x = pX
  mob.y = pY
  mob.vx = pVx
  mob.vy = pVy
  mob.supprime = false
  table.insert(mobManager.liste_mobs, mob)
  
  mob.Anime = function(dt)
  end
  
  mob.Deplace = function(dt)
    -- Application de la vélocité
    mob.x = mob.x + mob.vx * (dt*25)
    mob.y = mob.y + mob.vy * (dt*25)
  end
  
  return mob
  
end

mobManager.update = function(dt)
  local n
  for n=#mobManager.liste_mobs,1,-1 do
    local mob = mobManager.liste_mobs[n]
    
    local nColonneCollision
    local nLigneCollision
    nColonneCollision = math.floor((mob.x / LARGEURTILE) + 1)
    nLigneCollision = math.floor((mob.y / HAUTEURTILE) + 1)
    
    local bSupprimeDeLaListe = false
    if gameState.mapMurs[nLigneCollision][nColonneCollision] > 0 then
      print("Collision avec un mur")
    end
    
    mob.Deplace(dt)
    mob.Anime(dt)
  end
end

mobManager.draw = function()
  local n
  for n=1,#mobManager.liste_mobs do
    local s = mobManager.liste_mobs[n]
    if s.type == 1 then
      local flip = 1
      love.graphics.draw(imgMouche, s.x, s.y, 0, flip, 1, imgMouche:getWidth()/2, imgMouche:getHeight()/2)
    end
  end
end

return mobManager