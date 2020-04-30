local particleManager = {}

local gameState = require("gamestate")

particleManager.liste_particles = {}

particleManager.Reset = function()
  particleManager.liste_particles = {}
end

particleManager.CreeExplosion = function(pType, pX, pY)
  print("CreeExplosion",pType,pX,pY)
  if pType == "touchesol" then
    local n
    for n=1,5 do
      particleManager.CreeParticle(pType, pX, pY, math.random(-30,30)/10,math.random(-10,-40)/10,math.random(5,15))
    end
  elseif pType == "touchemur" then
    local n
    for n=1,10 do
      particleManager.CreeParticle(pType, pX, pY, math.random(-30,30)/10,math.random(-30,30)/10,math.random(5,25))
    end
  end

end

particleManager.CreeParticle = function(pType, pX, pY, pVx, pVy, pLife)
  
  if pLife == nil then
    pLife = 50
  end
  
  local particle = {}
  particle.type = pType
  particle.x = pX
  particle.y = pY
  particle.vx = pVx
  particle.vy = pVy
  particle.life = pLife
  particle.supprime = false
  table.insert(particleManager.liste_particles, particle)
  
  particle.Anime = function(dt)
  end
  
  particle.Deplace = function(dt)
    -- Application de la vélocité
    particle.x = particle.x + particle.vx * (dt*20)
    particle.y = particle.y + particle.vy * (dt*20)
    particle.life = particle.life - 1
    if particle.life < 0 then
      particle.supprime = true
    end
  end
  
  return particle
  
end

particleManager.update = function(dt)
  local n
  for n=#particleManager.liste_particles,1,-1 do
    local s = particleManager.liste_particles[n]
       
    if s.supprime == true then
      table.remove(particleManager.liste_particles,n)
    end
    
    s.Deplace(dt)
    s.Anime(dt)
  end
end

particleManager.draw = function()
  local n
  for n=1,#particleManager.liste_particles do
    local s = particleManager.liste_particles[n]
    --love.graphics.draw(frame, s.x, s.y, 0, s.flip, 1, s.l/2, s.h/2)
    love.graphics.circle("fill",s.x,s.y,1)
  end
end

return particleManager