function CreeAliens()
  local Aliens = {}
  Aliens.nombre = 0
  
  Aliens.AjouteAlien = function()
  -- Variante de : function Aliens.AjouteAlien()
    Aliens.nombre = Aliens.nombre + 1
    print("Module 3 / Il y a maintenant",Aliens.nombre,"aliens")
  end
  return Aliens
end

