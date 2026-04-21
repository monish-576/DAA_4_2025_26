const query = `
  [out:json];
  (
    node["name"](30.760, 76.565, 30.776, 76.585);
    way["name"](30.760, 76.565, 30.776, 76.585);
  );
  out center;
`;
fetch('https://overpass-api.de/api/interpreter', {
  method: 'POST',
  headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
  body: 'data=' + encodeURIComponent(query)
})
.then(res => res.json())
.then(data => {
  const items = data.elements.map(e => ({
    name: e.tags.name,
    type: e.tags.amenity || e.tags.building || 'unknown',
    lat: e.lat || e.center?.lat,
    lng: e.lon || e.center?.lon
  }));
  console.log(items.filter(i => i.name.toLowerCase().includes('block') || i.name.toLowerCase().includes('hostel') || i.name.toLowerCase().includes('academic') || i.type === 'cafe' || i.type === 'restaurant').map(i => `${i.name} (${i.type}): ${i.lat}, ${i.lng}`).join('\n'));
})
.catch(e => console.error(e));
