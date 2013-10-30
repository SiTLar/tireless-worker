char pcDefaultCfgFile[] = "\
<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n \
<root> \
  <handlers/> \
  <config> \
    <Paths> \
      <home type=\"Dir\" descr=\"Application home directory\" help=\"The home directory. All other paths are relative to that one. Leave the value empty to use the default.\" value=\"\"/> \
      <scripts type=\"Dir\" descr=\"Look for task scripts\" help=\"A directory to look for task scripts first. The value is relative to the home derictory.\" value=\"\"/> \
      <logs type=\"Dir\" descr=\"Store the logs\" help=\"A directory to store logfiles. The value is relative to the home derictory.\" value=\"\"/> \
      <handlers type=\"Dir\" descr=\"Look for task handler libraries\" help=\"A directory to look for dynamic libraries containing device and logging handlers. The value is relative to the home derictory.\" value=\"\"/> \
      <res type=\"Dir\" descr=\"Resources directory\" help=\"Normaly should not be changed. Consult the manual for detales.\" value=\"\"/> \
    </Paths> \
    <Internals> \
      <maxtools type=\"Int\" descr=\"Maximum tools quantity\" help=\"Maximnm number of tools allowed on the toolbar\" value=\"10\"/> \
      <maxdynmenu type=\"Int\" descr=\"Maximum save recent tasks\" help=\"Maximnm number of tasks been saved in the menu\" value=\"4\"/> \
      <timequant type=\"Int\" help=\"Time in milliseconds to wait between checking for conditions to be changed during tasks are sleeping or waiting for messages\" descr=\"Time in milliseconds between internal checks\" value=\"500\"/> \
    </Internals> \
  </config> \
  <tools/> \
</root>\
";
