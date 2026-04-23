export class Graph {
  constructor() {
    this.nodes = new Map();
    this.adjList = new Map();
  }

  addNode(id, name, lat, lng) {
    this.nodes.set(id, { id, name, lat, lng });
    this.adjList.set(id, []);
  }

  addEdge(u, v, distance) {
    this.adjList.get(u).push({ destId: v, distance });
    this.adjList.get(v).push({ destId: u, distance });
  }

  getNode(id) {
    return this.nodes.get(id);
  }

  dijkstra(startId, endId) {
    if (!this.nodes.has(startId) || !this.nodes.has(endId)) {
      return { found: false };
    }

    const dist = new Map();
    const parent = new Map();
    const unvisited = new Set();

    for (const id of this.nodes.keys()) {
      dist.set(id, Infinity);
      unvisited.add(id);
    }
    dist.set(startId, 0);

    while (unvisited.size > 0) {
      let u = null;
      let minDist = Infinity;
      unvisited.forEach(nodeId => {
        if (dist.get(nodeId) < minDist) {
          minDist = dist.get(nodeId);
          u = nodeId;
        }
      });

      if (u === null || minDist === Infinity) break;
      if (u === endId) break; 

      unvisited.delete(u);

      const neighbors = this.adjList.get(u);
      for (const edge of neighbors) {
        if (!unvisited.has(edge.destId)) continue;
        const alt = dist.get(u) + edge.distance;
        if (alt < dist.get(edge.destId)) {
          dist.set(edge.destId, alt);
          parent.set(edge.destId, u);
        }
      }
    }

    if (dist.get(endId) === Infinity) return { found: false };

    const path = [];
    let curr = endId;
    while (curr !== undefined && curr !== startId) {
      path.push(curr);
      curr = parent.get(curr);
    }
    path.push(startId);
    path.reverse();

    return {
      found: true,
      path,
      totalDistance: dist.get(endId)
    };
  }
}
