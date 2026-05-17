#!/usr/bin/env python3
"""
test_firebase.py
Sends dummy data to Firebase matching the actual schema (export.json).
No PIC or serial needed. Run to verify credentials + DB connection.
"""

import time
import firebase_admin
from firebase_admin import credentials, db
from datetime import datetime, timezone

FIREBASE_CRED   = "/home/akai2004/firebase-key.json"
FIREBASE_DB_URL = "https://YOUR-PROJECT-default-rtdb.firebaseio.com"

cred = credentials.Certificate(FIREBASE_CRED)
firebase_admin.initialize_app(cred, {"databaseURL": FIREBASE_DB_URL})

now_ms  = int(time.time() * 1000)
now_utc = datetime.now(timezone.utc).strftime("%Y-%m-%d %H:%M:%S UTC")

# ── sensors/ ─────────────────────────────────────────────────────────────────
db.reference("sensors").set({
    "soil_moisture_pct": 65,
    "temperature":       27.3,
    "humidity":          58,
    "current":           0.12345,   # amps
})
print("sensors/ ✓")

# ── status/ ──────────────────────────────────────────────────────────────────
db.reference("status").set({
    "gantry_x":       0,
    "mode":           "AUTOMATIC",
    "mode_changed_at": now_ms,
    "pump":           "OFF",
    "system_state":   "NORMAL",
})
print("status/ ✓")

# ── ai/latest_detection/ ─────────────────────────────────────────────────────
db.reference("ai/latest_detection").set({
    "class":      "Healthy",
    "confidence": 0.95,
    "image_url":  "https://example.com/test.jpg",
    "timestamp":  now_utc,
})
print("ai/latest_detection/ ✓")

# ── alerts/history/ ──────────────────────────────────────────────────────────
db.reference("alerts/history").push({
    "message":   "Test alert from pi_controller test script",
    "severity":  "info",
    "timestamp": now_ms,
})
print("alerts/history/ ✓")

# ── commands/ (safe defaults) ─────────────────────────────────────────────────
db.reference("commands").update({
    "emergency_stop":  False,
    "cancel_ai_protocol": False,
    "pump": {
        "state":     "OFF",
        "source":    "test",
        "timestamp": now_ms,
    },
    "gantry_move": {
        "x":         0,
        "source":    "test",
        "timestamp": now_ms,
    },
})
print("commands/ ✓")

print("\nAll done — check Firebase Console → Realtime Database")
