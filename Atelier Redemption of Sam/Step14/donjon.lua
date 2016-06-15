local donjon = {}
donjon.nombreDeColonnes = 9
donjon.nombreDeLignes = 6
donjon.salleDepart = nil
donjon.salleBoss = nil
donjon.listeSalles = {}
donjon.map = {}

local imageCrane = love.graphics.newImage("images/skull.png")

local function CreeSalle(pLigne,pColonne)
  local newSalle = {}
  
  newSalle.ligne = pLigne
  newSalle.colonne = pColonne
  
  newSalle.estOuverte = false
  
  newSalle.porteHaut = false
  newSalle.porteDroite = false
  newSalle.porteBas = false
  newSalle.porteGauche = false
  
  return newSalle
end

donjon.nombreDePortes = function(pSalle)
  local nombre = 0
  if pSalle.porteHaut == true then nombre = nombre+1 end
  if pSalle.porteDroite == true then nombre = nombre+1 end
  if pSalle.porteBas == true then nombre = nombre+1 end
  if pSalle.porteGauche == true then nombre = nombre+1 end
  return nombre
end

donjon.salleLaPlusLoin = function()
  local salleLoin = {}
  local distanceMax = 0
  
  salleLoin = donjon.salleDepart
  
  for n,s in pairs(donjon.listeSalles) do
    local distance = math.dist(donjon.salleDepart.colonne,donjon.salleDepart.ligne,s.colonne,s.ligne)
    if distance > distanceMax then
      distanceMax = distance
      salleLoin = s
    end
  end
  
  print("La salle la plus éloignée est à",salleLoin.colonne,salleLoin.ligne)
  return salleLoin
end

donjon.GenereDonjon = function()
  print("Génère le donjon")
  donjon.map = {}
  
  local nLigne,nColonne
  for nLigne=1,donjon.nombreDeLignes do
    donjon.map[nLigne] = {}
    for nColonne=1,donjon.nombreDeColonnes do
      donjon.map[nLigne][nColonne] = CreeSalle(nLigne,nColonne)
    end
  end
  
  -- Génère le donjon
  
  donjon.listeSalles = {}
  local nbSalles = 10
  
  -- Crée la salle de départ
  local nLigneDepart,nColonneDepart
  nLigneDepart = math.random(1, donjon.nombreDeLignes)
  nColonneDepart = math.random(1, donjon.nombreDeColonnes)
  local salleDepart = donjon.map[nLigneDepart][nColonneDepart]
  salleDepart.estOuverte = true
  table.insert(donjon.listeSalles,salleDepart)
  -- Mémoriser la salle de départ
  donjon.salleDepart = salleDepart
  
  while #donjon.listeSalles < nbSalles do
    
    -- Sélectionne une salle dans la liste
    local nSalle = math.random(1, #donjon.listeSalles)
    local nLigne = donjon.listeSalles[nSalle].ligne
    local nColonne = donjon.listeSalles[nSalle].colonne
    local salle = donjon.listeSalles[nSalle]
    local nouvelleSalle = nil
    
    local direction = math.random(1,4)
    
    local bAjouteSalle = false

    if direction == 1 and nLigne > 1 then
      -- Haut
      nouvelleSalle = donjon.map[nLigne-1][nColonne]
      if nouvelleSalle.estOuverte == false then
        salle.porteHaut = true
        nouvelleSalle.porteBas = true
        bAjouteSalle = true
      end
    elseif direction == 2 and nColonne < donjon.nombreDeColonnes then
      -- Droite
      nouvelleSalle = donjon.map[nLigne][nColonne+1]
      if nouvelleSalle.estOuverte == false then
        salle.porteDroite = true
        nouvelleSalle.porteGauche = true
        bAjouteSalle = true
      end
    elseif direction == 3 and nLigne < donjon.nombreDeLignes then
      -- Bas
      nouvelleSalle = donjon.map[nLigne+1][nColonne]
      if nouvelleSalle.estOuverte == false then
        salle.porteBas = true
        nouvelleSalle.porteHaut = true
        bAjouteSalle = true
      end
    elseif direction == 4 and nColonne > 1 then
      -- Gauche
      nouvelleSalle = donjon.map[nLigne][nColonne-1]
      if nouvelleSalle.estOuverte == false then
        salle.porteGauche = true
        nouvelleSalle.porteDroite = true
        bAjouteSalle = true
      end
    end
    
    -- Ajoute la salle
    if bAjouteSalle == true then
        nouvelleSalle.estOuverte = true
        table.insert(donjon.listeSalles, nouvelleSalle)      
    end
    
  end
  
  print("Donjon généré avec",#donjon.listeSalles,"salles")
end

donjon.DessineMapDonjon = function(pSalle)
  local x,y
  x=15
  y=15
  local largeurCase = 34
  local hauteurCase = 13
  local espaceCase = 5
  local nLigne,nColonne
  local xBoss, yBoss
  for nLigne=1,donjon.nombreDeLignes do
    x = 5
    for nColonne=1,donjon.nombreDeColonnes do
      local salle = donjon.map[nLigne][nColonne]
      if salle.estOuverte == false then
        love.graphics.setColor(60,60,60)
        love.graphics.rectangle("fill",x,y,largeurCase,hauteurCase)
      else
        -- Dessin de la salle en vert
        if pSalle == salle then
          love.graphics.setColor(25,255,25)
        else
          love.graphics.setColor(255,255,255)
        end
        love.graphics.rectangle("fill",x,y,largeurCase,hauteurCase)
        love.graphics.setColor(255,50,50)
        if salle.porteHaut == true then
          love.graphics.rectangle("fill",(x+largeurCase/2)-2,y-2,4,6)
        end
        if salle.porteDroite == true then
          love.graphics.rectangle("fill",(x+largeurCase)-2,(y+hauteurCase/2)-2,6,4)
        end
        if salle.porteBas == true then
          love.graphics.rectangle("fill",(x+largeurCase/2)-2,(y+hauteurCase)-2,4,6)
        end
        if salle.porteGauche then
          love.graphics.rectangle("fill",(x-2),(y+hauteurCase/2)-2,6,4)
        end
        if salle == donjon.salleBoss then
          xBoss = x + (largeurCase/2)
          yBoss = y + (hauteurCase/2)
        end
        love.graphics.setColor(255,255,255)
      end
      x = x + largeurCase + espaceCase
    end
    y = y + hauteurCase + espaceCase
  end
  love.graphics.setColor(255,255,255)
  
  -- Dessine le crane sur la case de la salle du boss
  love.graphics.draw(imageCrane,xBoss-(imageCrane:getWidth()/2),yBoss-(imageCrane:getHeight()/2))

end

return donjon