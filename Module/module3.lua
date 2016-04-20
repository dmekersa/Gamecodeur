function CreeAlien()
  local Aliens = {}
  Aliens.nombre = 0
  
  Aliens.AjouteAlien = function()
    Aliens.nombre = Aliens.nombre + 1
    print("Module 3 / Il y a maintenant",Aliens.nombre,"aliens")
  end
  return Aliens
end

