-- Cette ligne permet d'afficher des traces dans la console pendant l'éxécution
io.stdout:setvbuf('no')

-- Empèche Love de filtrer les contours des images quand elles sont redimentionnées
-- Indispensable pour du pixel art
love.graphics.setDefaultFilter("nearest")

-- Cette ligne permet de déboguer pas à pas dans ZeroBraneStudio
if arg[#arg] == "-debug" then require("mobdebug").start() end

heros = {}

math.randomseed(love.timer.getTime())

-- Listes d'éléments
liste_sprites = {}
liste_tirs = {}
liste_aliens = {}

-- Niveau 16x12
niveau = {}
table.insert(niveau, { 0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0 })
table.insert(niveau, { 0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0 })
table.insert(niveau, { 0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0 })
table.insert(niveau, { 0,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0 })
table.insert(niveau, { 0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0 })
table.insert(niveau, { 0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0 })
table.insert(niveau, { 0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0 })
table.insert(niveau, { 0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0 })
table.insert(niveau, { 0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0 })
table.insert(niveau, { 0,0,0,0,0,0,2,2,2,2,2,2,2,2,0,0 })
table.insert(niveau, { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 })
table.insert(niveau, { 3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3 })

-- Camera
camera = {}
camera.y = 0

-- Images des tuiles
imgTuiles = {}
local n
for n=1,3 do
  imgTuiles[n] = love.graphics.newImage("images/tuile_"..n..".png")
end

sonShoot = love.audio.newSource("sons/shoot.wav","static")

function CreeAlien(pType, pX, pY)
  
  local nomImage = ""
  if pType == 1 then
    nomImage = "enemy1"
  elseif pType == 2 then
    nomImage = "enemy2"
  end
  
  local alien = CreeSprite(nomImage, pX, pY)
  
  if pType == 1 then
    alien.vy = 2
    alien.vx = 0
  elseif pType == 2 then
    alien.vy = 2
    local direction = math.random(1,2)
    if direction == 1 then
      alien.vx = 1
    else
      alien.vx = -1
    end
  end

  table.insert(liste_aliens, alien)
end

function CreeSprite(pNomImage, pX, pY)
  
  sprite = {}
  sprite.x = pX
  sprite.y = pY
  sprite.supprime = false
  sprite.image = love.graphics.newImage("images/"..pNomImage..".png")
  sprite.l = sprite.image:getWidth()
  sprite.h = sprite.image:getHeight()
  
  table.insert(liste_sprites, sprite)
  
  return sprite
  
end

function love.load()
  
  love.window.setMode(1024,768)
  love.window.setTitle("Atelier Shooter Gamecodeur")
  
  largeur = love.graphics.getWidth()
  hauteur = love.graphics.getHeight()
  
  heros = CreeSprite("heros", largeur/2, hauteur/2)
  
  DemarreJeu()
    
end

function DemarreJeu()
  heros.x = largeur/2
  heros.y = hauteur - (heros.h*2)
  
  -- Création des aliens
  CreeAlien(1, largeur/2, 100)
  CreeAlien(2, largeur/2, 50)
  
  -- RAZ de la Caméra
  camera.y = 0
end

function love.update(dt)
  
  -- Avance la camera
  camera.y = camera.y + 1
  
  local n

  -- traitement des tirs
  for n=#liste_tirs,1,-1 do
    local tir = liste_tirs[n]
    tir.y = tir.y + tir.v
    
    -- Vérifier si le tir n'est pas sorti de l'écran
    if tir.y < 0 or tir.y > hauteur then
      
      -- Marque le sprite pour le supprimer plus tard
      tir.supprime = true
      table.remove(liste_tirs, n)
      
    end
  end
  
  -- traitement des aliens
  for n=#liste_aliens,1,-1 do
    local alien = liste_aliens[n]
    
    alien.x = alien.x + alien.vx
    alien.y = alien.y + alien.vy
    
    if alien.y > hauteur then
      alien.supprime = true
      table.remove(liste_aliens, n)
    end
  end
  
  -- Purge des sprites à supprimer
  for n=#liste_sprites,1,-1 do
    if liste_sprites[n].supprime == true then
      table.remove(liste_sprites,n)
    end
  end

  if love.keyboard.isDown("right") and heros.x < largeur then
    heros.x = heros.x + 2
  end

  if love.keyboard.isDown("left") and heros.x > 0 then
    heros.x = heros.x - 2
  end
  
  if love.keyboard.isDown("up") and heros.y > 0 then
    heros.y = heros.y - 2
  end

  if love.keyboard.isDown("down") and heros.y < hauteur then
    heros.y = heros.y + 2
  end

end

function love.draw()
  
  -- Dessin du niveau
  local nbLignes = #niveau
  local ligne,colonne
  local x,y
  
  x=0
  y=(0 - 64) + camera.y
  for ligne=nbLignes,1,-1 do
    for colonne=1,16 do
      -- Dessine la tuile
      local tuile = niveau[ligne][colonne]
      if tuile > 0 then
        love.graphics.draw(imgTuiles[tuile],x,y,0,2,2)
      end
      x = x + 64
    end
    x = 0
    y = y - 64
  end
  
  local n
  for n=1,#liste_sprites do
    local s = liste_sprites[n]
    love.graphics.draw(s.image, s.x, s.y, 0, 2, 2, s.l/2, s.h/2)
  end
  
  love.graphics.print("Nombre de tirs = "..#liste_tirs.." Nombre de sprites = "..#liste_sprites.." Nombre d'aliens = "..#liste_aliens,0,0)
    
end

function love.keypressed(key)
  
  if key == " " then
    local tir = CreeSprite("laser1", heros.x, heros.y - (heros.h*2)/2)
    tir.v = -10
    table.insert(liste_tirs, tir)
    
    sonShoot:play()
  end
  
end
  