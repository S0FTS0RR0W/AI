#Ai server using torch and api fastapi
import torch
from fastapi import FastAPI, HTTPException
from pydantic import BaseModel
from typing import List
import uvicorn

app = FastAPI()
model = None

class PredictionRequest(BaseModel):
    inputs: List[float]
class PredictionResponse(BaseModel):
    outputs: List[float]
@app.on_event("startup")
async def load_model():
    global model
    try:
        model = torch.load("model.pth")
        model.eval()
        print("Model loaded successfully")
    except Exception as e:
        model = None
        print(f"Error loading model: {e}")
        print("Server will start without a model. The /predict endpoint will not work.")
@app.post("/predict", response_model=PredictionResponse)
async def predict(request: PredictionRequest):
    if model is None:
        raise HTTPException(status_code=503, detail="Model is not loaded. Please check server logs.")
    inputs = torch.tensor(request.inputs).unsqueeze(0)
    with torch.no_grad():
        outputs = model(inputs)
    return PredictionResponse(outputs=outputs.tolist()[0])
if __name__ == "__main__":
    uvicorn.run(app, host="0.0.0.0", port=8000)