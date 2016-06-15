-- Cette ligne permet d'afficher des traces dans la console pendant l'éxécution
io.stdout:setvbuf('no')

-- Empèche Love de filtrer les contours des images quand elles sont redimentionnées
-- Indispensable pour du pixel art
love.graphics.setDefaultFilter("nearest")

-- Cette ligne permet de déboguer pas à pas dans ZeroBraneStudio
if arg[#arg] == "-debug" then require("mobdebug").start() end

local liste_etoiles = {}

function love.load()
  
  largeur = love.graphics.getWidth()
  hauteur = love.graphics.getHeight()
  
  local n
  for n=1,1000 do
    local etoile = {}
    etoile.x = math.random(1,largeur)
    etoile.y = math.random(1,hauteur)
    etoile.vitesse = math.random(20,60)/10
    table.insert(liste_etoiles,etoile)
  end
  
end

function love.update(dt)
  for n,etoile in pairs(liste_etoiles) do
    etoile.y = etoile.y + etoile.vitesse
    if etoile.y > hauteur then
      etoile.y = 1
    end
  end
end

function love.draw()
  for n,etoile in pairs(liste_etoiles) do
    love.graphics.circle("fill",etoile.x,etoile.y,1)
  end
end

function love.keypressed(key)
  
  print(key)
  
end