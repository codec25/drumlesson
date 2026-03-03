const CACHE_NAME = "drum-academy-v3";
const ASSETS = [
  "./",
  "./index.html",
  "./manifest.webmanifest",
  "./app-icon.svg",
  "https://cdn.tailwindcss.com",
  "https://unpkg.com/tone@14.8.49/build/Tone.js",
  "https://unpkg.com/react@18/umd/react.development.js",
  "https://unpkg.com/react-dom@18/umd/react-dom.development.js",
  "https://unpkg.com/@babel/standalone/babel.min.js"
];

self.addEventListener("install", (event) => {
  event.waitUntil(
    caches.open(CACHE_NAME).then(async (cache) => {
      for (const asset of ASSETS) {
        try { await cache.add(asset); } catch {}
      }
    })
  );
  self.skipWaiting();
});

self.addEventListener("activate", (event) => {
  event.waitUntil(
    caches.keys().then((keys) =>
      Promise.all(keys.filter((k) => k !== CACHE_NAME).map((k) => caches.delete(k)))
    )
  );
  self.clients.claim();
});

self.addEventListener("fetch", (event) => {
  const { request } = event;
  if (request.method !== "GET") return;

  // Network-first for document so new fixes ship immediately.
  if (request.mode === "navigate") {
    event.respondWith(
      fetch(request)
        .then((response) => {
          const copy = response.clone();
          caches.open(CACHE_NAME).then((cache) => cache.put(request, copy));
          return response;
        })
        .catch(() => caches.match("./index.html"))
    );
    return;
  }

  event.respondWith(
    caches.match(request).then((cached) => {
      if (cached) return cached;
      return fetch(request)
        .then((response) => {
          const copy = response.clone();
          caches.open(CACHE_NAME).then((cache) => cache.put(request, copy));
          return response;
        })
        .catch(() => caches.match("./index.html"));
    })
  );
});
