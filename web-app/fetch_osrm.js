async function getCU() {
  const query = `
    [out:json];
    (
      way(30.762,76.567,30.775,76.580)["name"~"(?i)block|hostel|academic"];
      node(30.762,76.567,30.775,76.580)["name"~"(?i)block|hostel|academic"];
    );
    out center;
  `;
  try {
    const res = await fetch('https://overpass-api.de/api/interpreter', {
      method: 'POST',
      headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
      body: 'data=' + encodeURIComponent(query)
    });
    const data = await res.json();
    const items = data.elements.map(e => ({ name: e.tags.name, lat: e.lat || e.center?.lat, lng: e.lon || e.center?.lon }));
    console.log(JSON.stringify(items, null, 2));
  } catch (e) {
    console.error(e);
  }
}
getCU();
