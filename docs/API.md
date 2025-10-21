# OfficeLabs Agent API Reference

Complete API documentation for the OfficeLabs Agent service.

## Base URL

```
http://localhost:8765
```

## Authentication

No authentication required for MVP (localhost only).

Future versions will support API keys.

## Endpoints

### Health Check

#### `GET /health`

Check if the service is running and healthy.

**Response:**
```json
{
  "status": "healthy",
  "llm_provider": "claude"
}
```

**Status Codes:**
- `200 OK` - Service is healthy

---

### Chat

#### `POST /chat`

Send a chat message to the AI assistant.

**Request Body:**
```json
{
  "message": "Rewrite @Para#3 to be more concise",
  "document_context": {
    "title": "My Document",
    "type": "writer",
    "url": "file:///path/to/document.odt"
  },
  "history": [
    {
      "role": "user",
      "content": "Hello"
    },
    {
      "role": "assistant",
      "content": "Hi! How can I help?"
    }
  ]
}
```

**Request Fields:**
- `message` (string, required) - The user's message
- `document_context` (object, optional) - Current document metadata
  - `title` (string) - Document title
  - `type` (string) - Document type: "writer", "calc", or "impress"
  - `url` (string) - Document file URL
- `history` (array, optional) - Previous conversation messages
  - `role` (string) - "user" or "assistant"
  - `content` (string) - Message content

**Response:**
```json
{
  "message": "Here's a more concise version of paragraph 3: ...",
  "patch": {
    "type": "replace",
    "target": "@Para#3",
    "old_value": "The implementation was done...",
    "new_value": "The implementation was complex.",
    "diff": "- The implementation was done...\n+ The implementation was complex."
  },
  "metadata": {
    "tokens_used": 250,
    "model": "claude-3-5-sonnet-20241022"
  }
}
```

**Response Fields:**
- `message` (string) - AI assistant's response
- `patch` (object, optional) - Patch to apply if modification requested
- `metadata` (object) - Response metadata
  - `tokens_used` (number) - Total tokens consumed
  - `model` (string) - Model used

**Status Codes:**
- `200 OK` - Success
- `400 Bad Request` - Invalid request
- `500 Internal Server Error` - Server error

**Example:**
```bash
curl -X POST http://localhost:8765/chat \
  -H "Content-Type: application/json" \
  -d '{
    "message": "Summarize this document",
    "document_context": {
      "title": "Report.odt",
      "type": "writer"
    }
  }'
```

---

### Document Index

#### `POST /index/update`

Update the document index with new object mappings.

**Request Body:**
```json
{
  "document_url": "file:///path/to/document.odt",
  "document_type": "writer",
  "objects": [
    {
      "type": "heading",
      "address": "@Heading(\"Introduction\")",
      "display": "Heading: Introduction",
      "preview": "This document introduces...",
      "metadata": {
        "level": 1
      }
    },
    {
      "type": "paragraph",
      "address": "@Para#1",
      "display": "Paragraph 1",
      "preview": "First paragraph content...",
      "metadata": {
        "number": 1
      }
    }
  ],
  "metadata": {
    "page_count": 5,
    "word_count": 1234
  }
}
```

**Request Fields:**
- `document_url` (string, required) - Document file URL
- `document_type` (string, required) - "writer", "calc", or "impress"
- `objects` (array, required) - Array of document objects
  - `type` (string) - Object type
  - `address` (string) - @-address
  - `display` (string) - Display name
  - `preview` (string) - Content preview
  - `metadata` (object) - Additional metadata
- `metadata` (object, optional) - Document-level metadata

**Response:**
```json
{
  "status": "success",
  "object_count": 42,
  "message": "Indexed 42 objects from writer document"
}
```

**Status Codes:**
- `200 OK` - Index updated successfully
- `400 Bad Request` - Invalid request
- `500 Internal Server Error` - Server error

---

#### `GET /index/objects`

Get @-addressable objects for autocomplete.

**Query Parameters:**
- `query` (string, optional) - Search query to filter objects
- `type` (string, optional) - Filter by object type

**Response:**
```json
[
  {
    "type": "heading",
    "address": "@Heading(\"Introduction\")",
    "display": "Heading: Introduction",
    "preview": "This document introduces..."
  },
  {
    "type": "paragraph",
    "address": "@Para#3",
    "display": "Paragraph 3",
    "preview": "The third paragraph..."
  }
]
```

**Response:** Array of objects (max 20 results)

**Status Codes:**
- `200 OK` - Success
- `500 Internal Server Error` - Server error

**Example:**
```bash
curl "http://localhost:8765/index/objects?query=intro&type=heading"
```

---

#### `GET /index/current`

Get the currently indexed document.

**Response:**
```json
{
  "document_url": "file:///path/to/document.odt",
  "document_type": "writer",
  "objects": [...],
  "metadata": {...}
}
```

**Status Codes:**
- `200 OK` - Success
- `404 Not Found` - No document indexed
- `500 Internal Server Error` - Server error

---

### Patches

#### `POST /patch/preview`

Generate a patch preview.

**Request Body:**
```json
{
  "type": "replace",
  "target": "@Para#3",
  "old_value": "The quick brown fox",
  "new_value": "The fast red fox"
}
```

**Request Fields:**
- `type` (string, required) - Patch type: "replace", "insert", or "delete"
- `target` (string, required) - @-address of target object
- `old_value` (string, optional) - Current value (for replace)
- `new_value` (string, required) - New value

**Response:**
```json
{
  "patch_id": "550e8400-e29b-41d4-a716-446655440000",
  "patch_type": "replace",
  "target": "@Para#3",
  "old_value": "The quick brown fox",
  "new_value": "The fast red fox",
  "diff": "- The quick brown fox\n+ The fast red fox"
}
```

**Response Fields:**
- `patch_id` (string) - UUID for this patch
- `patch_type` (string) - Patch type
- `target` (string) - Target @-address
- `old_value` (string) - Old value
- `new_value` (string) - New value
- `diff` (string) - Unified diff

**Status Codes:**
- `200 OK` - Preview generated
- `400 Bad Request` - Invalid patch
- `500 Internal Server Error` - Server error

---

#### `POST /patch/apply`

Apply a previously previewed patch.

**Request Body:**
```json
{
  "patch_id": "550e8400-e29b-41d4-a716-446655440000",
  "document_context": {
    "title": "My Document",
    "type": "writer",
    "url": "file:///path/to/document.odt"
  }
}
```

**Request Fields:**
- `patch_id` (string, required) - UUID of patch to apply
- `document_context` (object, required) - Current document context

**Response:**
```json
{
  "status": "success",
  "message": "Applied patch to @Para#3"
}
```

**Status Codes:**
- `200 OK` - Patch applied successfully
- `404 Not Found` - Patch ID not found
- `500 Internal Server Error` - Application error

---

## Error Responses

All error responses follow this format:

```json
{
  "detail": "Error message describing what went wrong"
}
```

**Common Error Codes:**
- `400 Bad Request` - Invalid input
- `404 Not Found` - Resource not found
- `500 Internal Server Error` - Server error
- `503 Service Unavailable` - LLM API unavailable

## Rate Limiting

No rate limiting in MVP.

Future versions will implement:
- Per-IP rate limits
- Per-API-key rate limits
- Token budget management

## Versioning

Current version: `v1` (implicit, no version in URL)

Future versions will use URL versioning:
- `http://localhost:8765/v2/chat`

## WebSocket Support

Not implemented in MVP.

Future versions will support:
- `ws://localhost:8765/ws/chat` - Streaming chat
- `ws://localhost:8765/ws/events` - Document events

## SDK Examples

### Python

```python
import requests

# Chat
response = requests.post(
    "http://localhost:8765/chat",
    json={
        "message": "Rewrite @Para#3",
        "document_context": {
            "title": "Doc",
            "type": "writer"
        }
    }
)
print(response.json()["message"])

# Update index
requests.post(
    "http://localhost:8765/index/update",
    json={
        "document_url": "file:///doc.odt",
        "document_type": "writer",
        "objects": [...]
    }
)

# Get autocomplete
objects = requests.get(
    "http://localhost:8765/index/objects",
    params={"query": "intro"}
).json()
```

### JavaScript/TypeScript

```typescript
// Chat
const response = await fetch("http://localhost:8765/chat", {
  method: "POST",
  headers: { "Content-Type": "application/json" },
  body: JSON.stringify({
    message: "Rewrite @Para#3",
    document_context: {
      title: "Doc",
      type: "writer"
    }
  })
});
const data = await response.json();
console.log(data.message);

// Update index
await fetch("http://localhost:8765/index/update", {
  method: "POST",
  headers: { "Content-Type": "application/json" },
  body: JSON.stringify({
    document_url: "file:///doc.odt",
    document_type: "writer",
    objects: [...]
  })
});
```

### cURL

```bash
# Health check
curl http://localhost:8765/health

# Chat
curl -X POST http://localhost:8765/chat \
  -H "Content-Type: application/json" \
  -d '{"message": "Hello"}'

# Get objects
curl "http://localhost:8765/index/objects?query=intro"

# Preview patch
curl -X POST http://localhost:8765/patch/preview \
  -H "Content-Type: application/json" \
  -d '{
    "type": "replace",
    "target": "@Para#3",
    "new_value": "New text"
  }'
```

## Testing

Use the included Postman collection:
```bash
# Import postman/OfficeLabs-Agent.postman_collection.json
```

Or test with pytest:
```bash
cd officelabs-agent
pytest tests/test_api.py
```

## OpenAPI Specification

Full OpenAPI spec available at:
```
http://localhost:8765/docs
```

Interactive API documentation (Swagger UI):
```
http://localhost:8765/docs
```

Alternative documentation (ReDoc):
```
http://localhost:8765/redoc
```

## Support

- Issues: https://github.com/officelabs/officelabs-agent/issues
- Discussions: https://github.com/officelabs/officelabs-agent/discussions

