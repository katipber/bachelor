package cfs.view;

import cfs.data.entity.Employee;
import cfs.data.entity.Trip;
import com.vaadin.ui.Button;
import com.vaadin.ui.FormLayout;
import com.vaadin.ui.Grid;
import com.vaadin.ui.HorizontalLayout;
import com.vaadin.ui.TextField;
import java.util.List;
import java.util.Optional;
import cfs.controller.ReservationControllerInterface;

/**
 * This view is used to display all of the trips.
 */

public class TripView extends HorizontalLayout {
  
  private ReservationControllerInterface controller;
  private Employee user;
  private Grid<Trip> grid;
  private TextField customer;
  private TextField description;
  private TextField start;
  private TextField end;
  private TextField car;
  private TextField state;

  public TripView(Employee user, ReservationControllerInterface controller) {
    this.controller = controller;
    this.user = user;
    setGrid();
    setInfo();
  }
  
  private void setGrid() {
    List<Trip> trips = controller.getTrips(user);
    grid = new Grid<>();
    grid.setItems(trips);
    grid.addColumn(Trip::getCustomer).setCaption("Customer");
    grid.addColumn(Trip::getStart_date).setCaption("Start Date");
    grid.addColumn(Trip::getCarPlate).setCaption("Car");
    grid.addColumn(Trip::getState).setCaption("State");
    grid.setWidth("66%");
    grid.setSelectionMode(Grid.SelectionMode.SINGLE);
    grid.addSelectionListener(e -> updateInfo(e.getFirstSelectedItem()));
    grid.setWidth("100%");
    
    addComponent(grid);
  }

  private void setInfo() {
    FormLayout form = new FormLayout();
    
    customer = new TextField("Customer");
    customer.setReadOnly(true);
    description = new TextField("Description");
    description.setReadOnly(true);
    start = new TextField("Start Date");
    start.setReadOnly(true);
    end = new TextField("End Date");
    end.setReadOnly(true);
    car = new TextField("Car");
    car.setReadOnly(true);
    state = new TextField("State");
    state.setReadOnly(true);
    
    Button cancelCar = new Button("Cancel Car");
    Button cancelTrip = new Button("Cancel Trip");
    cancelCar.setEnabled(false);
    cancelTrip.setEnabled(false);
    
    form.addComponents(customer, description, start, end, car, state, cancelCar, cancelTrip);
    addComponent(form);
  }

  private void updateInfo(Optional<Trip> trip) {
    clearForm();
    if(trip.isPresent()) {
      customer.setValue(trip.get().getCustomer());
      if(trip.get().getDescription() != null)
      description.setValue(trip.get().getDescription());
      start.setValue(trip.get().getStart_date().toString());
      end.setValue(trip.get().getEnd_date().toString());
      if(trip.get().getReservation() != null)
        car.setValue(trip.get().getReservation().getCar().getPlate());
      state.setValue(trip.get().getState());
    }
  }
  
  private void clearForm() {
    customer.clear();
    description.clear();
    start.clear();
    end.clear();
    car.clear();
    state.clear();
  }
}
