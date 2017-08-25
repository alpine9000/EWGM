

function on_uae_vsync()
   --io.write(uae_peek_symbol32("_hw_verticalBlankCount"), "\n");

   if ready == nil and uae_peek_symbol32("_hw_verticalBlankCount") == 2 then
      ready = true
   end

   if frData == nil and ready ~= nil and uae_peek_symbol16("_random_16") ~= 0 then
      frData = io.open("lua.data", "w")
   end

   if frData ~= nil then
      frData:write(uae_peek_symbol16("_random_16"), "\n");
   end

end
