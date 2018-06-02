package cfs.view;

import cfs.data.entity.Employee;
import cfs.data.entity.Trip;
import com.vaadin.ui.Button;
import com.vaadin.ui.CheckBox;
import com.vaadin.ui.DateField;
import com.vaadin.ui.FormLayout;
import com.vaadin.ui.Label;
import com.vaadin.ui.Notification;
import com.vaadin.ui.TextField;
import cfs.controller.ReservationControllerInterface;

/**
 * The view responsible for trip creation.
 * Contains all of the neccessary input fields
 * for the trip information
 */

public class NewTripView extends FormLayout { 

  private ReservationControllerInterface controller;
  private Employee  employee;
  
  private TextField customer;
  private TextField description;
  private DateField start;
  private DateField end;
  private CheckBox  reservation;

  NewTripView(Employee user, ReservationControllerInterface controller) {
    this.controller = controller;
    this.employee = user;
    setForm();
  }

  private void setForm() {
    customer = new TextField("Customer");
    description = new TextField("Description");
    Label startLabel = new Label("Start Date");
    Label endLabel = new Label("End Date");
    start = new DateField();
    end = new DateField();
    reservation = new CheckBox("Reserve Car");
    Button submit = new Button("Submit");
    submit.addClickListener(e -> createTrip());
    
    addComponents(customer, description, startLabel, start, endLabel, end, reservation, submit);
  }

  private void createTrip() {
    Trip trip = new Trip();
    trip.setEmployee(employee);
    trip.setCustomer(customer.getValue());
    trip.setDescription(description.getValue());
    trip.setStart_date(start.getValue());
    trip.setEnd_date(end.getValue());
    trip.setReservation_request(reservation.getValue());
    if(controller.createTripRequest(trip)) {
      Notification.show("Request Submitted!");
      clearFields();
    }
    else
      Notification.show("Invalid Request!", Notification.Type.ERROR_MESSAGE);
  }

  private void clearFields() {
    customer.clear();
    description.clear();
    start.clear();
    end.clear();
    reservation.clear();
  }
}
