def new_func():
    class PharmacyInventorySystem:
        def _init_(self):
            self.prescription_stack = []
            self.prescription_queue = []
            self.available_medications = {}

        def add_prescription(self, medication, quantity, date):
            prescription = {"medication": medication, "quantity": quantity, "date": date}
            self.prescription_stack.append(prescription)
            self.prescription_queue.append(prescription)
            print(f"Prescription added: {medication}, Quantity: {quantity}, Date: {date}")

        def process_prescription(self):
            if self.prescription_queue:
                prescription = self.prescription_queue.pop(0)
                medication = prescription["medication"]
                quantity = prescription["quantity"]
                if medication in self.available_medications:
                    if self.available_medications[medication]["quantity"] >= quantity:
                        self.available_medications[medication]["quantity"] -= quantity
                        print(f"Prescription processed: {medication}, Quantity: {quantity}")
                    else:
                        print(f"Insufficient quantity of {medication} in stock")
                else:
                    print(f"{medication} is not available in stock")
            else:
                print("No prescriptions to process")

        def undo_prescription(self):
            if self.prescription_stack:
                prescription = self.prescription_stack.pop()
                medication = prescription["medication"]
                quantity = prescription["quantity"]
                self.available_medications[medication]["quantity"] += quantity
                print(f"Prescription undone: {medication}, Quantity: {quantity}")
            else:
                print("No prescriptions to undo")

        def add_medication(self, medication, quantity, expiration_date):
            self.available_medications[medication] = {"quantity": quantity, "expiration_date": expiration_date}
            print(f"Medication added: {medication}, Quantity: {quantity}, Expiration Date: {expiration_date}")

        def display_system_output(self):
            print("Prescription Stack:")
            for prescription in self.prescription_stack:
                print(f"  {prescription['medication']}, Quantity: {prescription['quantity']}, Date: {prescription['date']}")

            print("\nPrescription Queue:")
            for prescription in self.prescription_queue:
                print(f"  {prescription['medication']}, Quantity: {prescription['quantity']}, Date: {prescription['date']}")

            print("\nAvailable Medications:")
            for medication, details in self.available_medications.items():
                print(f"  {medication}: Quantity: {details['quantity']}, Expiration Date: {details['expiration_date']}")
    
    if __name__ == "__main____":
        pharmacy_system = PharmacyInventorySystem()
        pharmacy_system.add_medication("Medication A", 50, "2023-03-31")
        pharmacy_system.add_medication("Medication B", 75, "2023-04-30")
        pharmacy_system.add_medication("Medication C", 100, "2023-05-31")
        pharmacy_system.add_medication("Medication D", 25, "2023-06-30")
        pharmacy_system.add_prescription("Medication A", 10, "2023-02-20")
        pharmacy_system.add_prescription("Medication B", 20, "2023-02-22")
        pharmacy_system.add_prescription("Medication C", 30, "2023-02-25")
        pharmacy_system.process_prescription()
        pharmacy_system.process_prescription()
        pharmacy_system.undo_prescription()
        pharmacy_system.display_system_output()
