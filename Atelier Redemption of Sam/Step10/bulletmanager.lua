local bulletManager = {}

local gameState = require("gamestate")

bulletManager.liste_bullets = {}

bulletManager.Reset = function()
  bulletManager.liste_bullets = {}
end

bulletManager.CreeBullet = function(pType, pX, pY, pVx, pVy, pLife)
  
  if pLife == nil then
    pLife = 50
  end
  
  local bullet = {}
  bullet.type = pType
  bullet.x = pX
  bullet.y = pY
  bullet.vx = pVx
  bullet.vy = pVy
  bullet.life = pLife
  bullet.supprime = false
  table.insert(bulletManager.liste_bullets, bullet)
  
  bullet.Anime = function(dt)
  end
  
  bullet.Deplace = function(dt)
    -- Application de la vélocité
    bullet.x = bullet.x + bullet.vx
    bullet.y = bullet.y + bullet.vy
    bullet.life = bullet.life - 1
    if bullet.life < 0 then
      bullet.supprime = true
    end
  end
  
  return bullet
  
end

bulletManager.update = function(dt)
  local n
  for n=#bulletManager.liste_bullets,1,-1 do
    local s = bulletManager.liste_bullets[n]
    
    local nColonneCollision
    local nLigneCollision
    nColonneCollision = math.floor((s.x / LARGEURTILE) + 1)
    nLigneCollision = math.floor((s.y / HAUTEURTILE) + 1)
    
    if gameState.mapMurs[nLigneCollision][nColonneCollision] > 0 or s.supprime == true then
      table.remove(bulletManager.liste_bullets,n)
    end
    
    s.Deplace(dt)
    s.Anime(dt)
  end
end

bulletManager.draw = function()
  local n
  for n=1,#bulletManager.liste_bullets do
    local s = bulletManager.liste_bullets[n]
    --love.graphics.draw(frame, s.x, s.y, 0, s.flip, 1, s.l/2, s.h/2)
    love.graphics.circle("fill",s.x,s.y,10)
  end
end

return bulletManager