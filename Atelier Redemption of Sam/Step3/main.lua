io.stdout:setvbuf('no')
if arg[#arg] == "-debug" then require("mobdebug").start() end

local spriteManager = require("spritemanager")

-- Globales utiles
local largeurEcran
local hauteurEcran

-- Constantes
local LARGEURTILE = 64
local HAUTEURTILE = 64

-- La salle, sans les portes
local salleSprite = love.graphics.newImage("images/salle.png")

-- Le héros !
local samSprite = {}

map_murs = {}
map_murs[1]  = {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2}
map_murs[2]  = {2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2}
map_murs[3]  = {2,1,0,0,0,0,0,0,0,0,0,0,0,0,1,2}
map_murs[4]  = {2,1,0,0,0,0,0,0,0,0,0,0,0,0,1,2}
map_murs[5]  = {2,1,0,0,0,0,0,0,0,0,0,0,0,0,1,2}
map_murs[6]  = {2,1,0,0,0,0,0,0,0,0,0,0,0,0,1,2}
map_murs[7]  = {2,1,0,0,0,0,0,0,0,0,0,0,0,0,1,2}
map_murs[8]  = {2,1,0,0,0,0,0,0,0,0,0,0,0,0,1,2}
map_murs[9]  = {2,1,0,0,0,0,0,0,0,0,0,0,0,0,1,2}
map_murs[10] = {2,1,0,0,0,0,0,0,0,0,0,0,0,0,1,2}
map_murs[11] = {2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2}
map_murs[12] = {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2}

-- Le donjon
local donjon = require("donjon")

local salleCourante = {}
local listeColliders = {}

function DemarreJeu()
  
  samSprite = spriteManager.CreeSprite("p1_walk",11,0,0)
  
  -- Centre Sam à l'écran
  samSprite.x = largeurEcran/2
  samSprite.y = hauteurEcran/2

  -- Génération d'un donjon
  donjon.GenereDonjon()
  ChargeSalle(donjon.salleDepart)
end

function DessineColliders()
  local n
  
  for n=1,#listeColliders do
    local c = listeColliders[n]
    love.graphics.rectangle("line",c.x,c.y,c.largeur,c.hauteur)
  end

end

function TesteColliders(pX,pY)
  local n
  
  for n=1,#listeColliders do
    local c = listeColliders[n]

    if pX > c.x and pX < c.x + c.largeur and
       pY > c.y and pY < c.y + c.hauteur then
       return c.type
    end

  end

  return ""
end

function AjouteCollider(pType,pX,pY,pLargeur,pHauteur)
  print("Ajoute un collider de type",pType,":",pX,pY,pLargeur,pHauteur)
  local collider = {}
  collider.type = pType
  collider.x = pX
  collider.y = pY
  collider.largeur = pLargeur
  collider.hauteur = pHauteur
  table.insert(listeColliders,collider)
end

function ChargeSalle(pSalle)
  print("Charge la salle")
  
  -- Référence de la salle dans la map
  salleCourante.salle = pSalle
  
  -- Zones de collision avec les portes
  listeColliders = {}
  
  if salleCourante.salle.porteHaut == true then
    AjouteCollider("porteHaut",(largeurEcran/2)-LARGEURTILE,0,LARGEURTILE*2,HAUTEURTILE*2)
  end
  if salleCourante.salle.porteGauche == true then
    AjouteCollider("porteGauche",0,(hauteurEcran/2)-HAUTEURTILE,LARGEURTILE*2,HAUTEURTILE*2)
  end
  if salleCourante.salle.porteDroite == true then
    AjouteCollider("porteDroite",largeurEcran-(LARGEURTILE*2),(hauteurEcran/2)-HAUTEURTILE,LARGEURTILE*2,HAUTEURTILE*2)
  end
  if salleCourante.salle.porteBas == true then
    AjouteCollider("porteBas",(largeurEcran/2)-LARGEURTILE,hauteurEcran-(HAUTEURTILE*2),LARGEURTILE*2,HAUTEURTILE*2)
  end

end

function love.load()
  
  love.window.setTitle( "Redemption of Sam (c) 2015 David Mekersa from Gamecodeur.fr" )
  love.window.setMode(1024, 768, {fullscreen=false, vsync=true}) --, minwidth=1024, minheight=768})
  
  largeurEcran = love.graphics.getWidth()
  hauteurEcran = love.graphics.getHeight()
  
  DemarreJeu()
  
end

function love.update(dt)

  -- Stocke la position actuelle de Sam
  local ancienX = samSprite.x
  local ancienY = samSprite.y

  spriteManager.update(dt)
  if math.abs(samSprite.vx) < 1 and math.abs(samSprite.vy) < 1 then
    samSprite.frame = 1
  end
  
  -- Détection des touches du clavier
  if love.keyboard.isDown("d") then
      samSprite.vx = samSprite.vx + 1
      samSprite.flip = 1
  end
  if love.keyboard.isDown("q") or love.keyboard.isDown("a") then
      samSprite.vx = samSprite.vx - 1
      samSprite.flip = -1
  end
  if love.keyboard.isDown("z") or love.keyboard.isDown("w") then
      samSprite.vy = samSprite.vy - 1
  end
  if love.keyboard.isDown("s") then
      samSprite.vy = samSprite.vy + 1
  end
  
  -- Detection de collision avec les portes
  local changeDeSalle = false
  local collisionType = TesteColliders(samSprite.x,samSprite.y)
  if collisionType ~= "" then
    print("Collision avec ",collisionType)
    local ligne = salleCourante.salle.ligne
    local colonne = salleCourante.salle.colonne
    if collisionType == "porteHaut" then
      salleCourante.salle = donjon.map[ligne-1][colonne]
      samSprite.y = hauteurEcran-(HAUTEURTILE*2)
      changeDeSalle = true
    elseif collisionType == "porteDroite" then
      samSprite.x = 0+(LARGEURTILE*2)
      salleCourante.salle = donjon.map[ligne][colonne+1]
      changeDeSalle = true
    elseif collisionType == "porteBas" then
      salleCourante.salle = donjon.map[ligne+1][colonne]
      samSprite.y = 0+(HAUTEURTILE*2)
      changeDeSalle = true
    elseif collisionType == "porteGauche" then
      samSprite.x = largeurEcran-(LARGEURTILE*2)
      salleCourante.salle = donjon.map[ligne][colonne-1]
      changeDeSalle = true
    end
    if changeDeSalle == true then
      ChargeSalle(salleCourante.salle)
    end
  end
  
  if changeDeSalle == false then
    -- Détection de collision avec les murs
    -- si collision alors on rétablit l'ancienne position
    local colonneCollision = math.floor(samSprite.x/LARGEURTILE)+1
    local ligneCollision = math.floor(samSprite.y/HAUTEURTILE)+1
    if map_murs[ligneCollision][colonneCollision] > 0 then
      samSprite.x = ancienX
      samSprite.y = ancienY
      samSprite.vx = 0
      samSprite.vy = 0
    end
  end
end

function love.draw()
  -- On dessine la salle en 1er
  love.graphics.draw(salleSprite,0,0)
  -- On dessine les sprites
  
  spriteManager.draw()
  
  --love.graphics.circle("fill",samSprite.x,samSprite.y,3)
  
  DessineColliders()

  donjon.DessineMapDonjon(salleCourante.salle.ligne,salleCourante.salle.colonne)

  -- Informations de debug
  love.graphics.print("Salle en cours : "..salleCourante.salle.ligne..","..salleCourante.salle.colonne)
end

function love.keypressed(key)
  if key == "space" or key == " " then
    DemarreJeu()
  end
end