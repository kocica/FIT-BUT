model_json = final_model.to_json()
with open("/content/drive/My Drive/MODEL-NAME.json", "w") as json_file:
    json_file.write(model_json)