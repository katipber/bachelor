package cfs.view;

import cfs.data.entity.Employee;
import cfs.data.entity.Reservation;
import com.vaadin.ui.Button;
import com.vaadin.ui.CheckBox;
import com.vaadin.ui.FormLayout;
import com.vaadin.ui.Grid;
import com.vaadin.ui.HorizontalLayout;
import com.vaadin.ui.Notification;
import com.vaadin.ui.TextField;
import java.time.LocalDate;
import java.util.List;
import java.util.Optional;
import java.util.Set;
import cfs.controller.ReservationControllerInterface;

/**
 * This view is used to display all of the reservations.
 * It uses the grid to display the list of items.
 */

public class ReservationView extends HorizontalLayout {
    
  private ReservationControllerInterface controller;
  private Employee user;
  private Grid<Reservation> grid;
  private TextField employee;
  private TextField rstart;
  private TextField rend;
  private TextField tstart;
  private TextField tend;
  private TextField car;
  private TextField state;
  
  private Button startButton;
  private Button endButton;
  private Button cancelButton;
  
  private CheckBox damagedCar;

  public ReservationView(Employee user, ReservationControllerInterface controller) {
    this.controller = controller;
    this.user = user;
    setGrid();
    setInfo();
  }
  
  private void setGrid() {
    List<Reservation> reservations = controller.getReservations();
    grid = new Grid<>();
    grid.setItems(reservations);
    grid.addColumn(Reservation::getEmployee).setCaption("Customer");
    grid.addColumn(Reservation::getTripStart_date).setCaption("Start Date");
    grid.addColumn(Reservation::getTripEnd_date).setCaption("End Date");
    grid.addColumn(Reservation::getCarPlate).setCaption("Car");
    grid.addColumn(Reservation::getState).setCaption("State");
    grid.setWidth("66%");
    grid.setSelectionMode(Grid.SelectionMode.SINGLE);
    grid.addSelectionListener(e -> updateInfo(e.getFirstSelectedItem()));
    grid.setWidth("100%");
    
    addComponent(grid);
  }

  private void setInfo() {
    FormLayout form = new FormLayout();
    
    employee = new TextField("Employee");
    employee.setReadOnly(true);
    rstart = new TextField("Reservation Start Date");
    rstart.setReadOnly(true);
    rend = new TextField("Reservation End Date");
    rend.setReadOnly(true);
    tstart = new TextField("Realization Start Date");
    tstart.setReadOnly(true);
    tend = new TextField("Realization End Date");
    tend.setReadOnly(true);
    car = new TextField("Car");
    car.setReadOnly(true);
    state = new TextField("State");
    state.setReadOnly(true);
    
    startButton = new Button("Start Trip");
    startButton.addClickListener(e -> startTrip());
    endButton = new Button("End Trip");
    endButton.addClickListener(e -> endTrip());
    cancelButton = new Button("Cancel Reservation");
    cancelButton.setEnabled(false);
    
    damagedCar = new CheckBox("Damaged Car");
    damagedCar.setValue(false);
    
    form.addComponents(employee, tstart, tend, rstart, rend, car, state, startButton, endButton, damagedCar, cancelButton);
    addComponent(form);
  }

  private void updateInfo(Optional<Reservation> res) {
    clearForm();
    if(res.isPresent()) {
      employee.setValue(res.get().getEmployee());
      rstart.setValue(res.get().getTripStart_date().toString());
      rend.setValue(res.get().getTripEnd_date().toString());

      if(res.get().getStart_date() != null) {
        tstart.setValue(res.get().getStart_date().toString());
        if(res.get().getEnd_date() == null)
          endButton.setEnabled(true);
      }
      else
        endButton.setEnabled(false);
      if(res.get().getEnd_date() != null) 
        tend.setValue(res.get().getEnd_date().toString());
      
      car.setValue(res.get().getCarPlate());
      state.setValue(res.get().getState());
      
      if(res.get().getStart_date()== null && !res.get().getTrip().getEnd_date().isBefore(LocalDate.now()))
        startButton.setEnabled(true);
      else
        startButton.setEnabled(false);
      
      damagedCar.setValue(false);
    }
  }
  
  private void clearForm() {
    employee.clear();
    tstart.clear();
    tend.clear();
    rstart.clear();
    rend.clear();
    car.clear();
    state.clear();
  }

  private void startTrip() {
    Set<Reservation> selectedItems = grid.getSelectedItems();
    for(Reservation r : selectedItems) {
      if(controller.startTrip(r))
        Notification.show("Trip Started!");
      else
        Notification.show("Cannot Start Trip!", Notification.Type.ERROR_MESSAGE);
      updateGrid();
      updateInfo(Optional.of(r));
    }
  }

  private void endTrip() {
    Set<Reservation> selectedItems = grid.getSelectedItems();
    for(Reservation r : selectedItems) {
      if(controller.endTrip(r, damagedCar.getValue()))
        Notification.show("Trip Ended!");
      else
        Notification.show("Cannot End Trip!", Notification.Type.ERROR_MESSAGE);
      updateGrid();
      updateInfo(Optional.of(r));
    }
  }

  private void updateGrid() {
    List<Reservation> reservations = controller.getReservations();
    grid.setItems(reservations);
  }
}
