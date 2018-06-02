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
import java.util.Set;
import cfs.controller.ReservationControllerInterface;

/**
 * This view is used to display all of the requests.
 * It uses the grid in order to list the requests.
 */

public class RequestView extends HorizontalLayout {

  private ReservationControllerInterface controller;
  private Employee   user;
  private Grid<Trip> grid;
  private TextField  employee;
  private TextField  customer;
  private TextField  description;
  private TextField  start;
  private TextField  end;
  private TextField  state;
  
  private Button acceptButton;
  private Button rejectButton;
  
  public RequestView(Employee user, ReservationControllerInterface controller) {
    this.controller = controller;
    this.user = user;
    setGrid();
    setInfo();
  }
  
  private void setGrid() {
    List<Trip> trips = controller.getTripsByDepartment(user.getManaging());
    grid = new Grid<>();
    grid.setItems(trips);
    grid.addColumn(Trip::getEmployeeEmail).setCaption("Employee");
    grid.addColumn(Trip::getCustomer).setCaption("Customer");
    grid.addColumn(Trip::getStart_date).setCaption("Start Date");
    grid.addColumn(Trip::getState).setCaption("State");
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
    customer = new TextField("Customer");
    customer.setReadOnly(true);
    description = new TextField("Description");
    description.setReadOnly(true);
    start = new TextField("Start Date");
    start.setReadOnly(true);
    end = new TextField("End Date");
    end.setReadOnly(true);
    state = new TextField("State");
    state.setReadOnly(true);
    
    acceptButton = new Button("Accept");
    acceptButton.addClickListener(e -> acceptRequest());
    rejectButton = new Button("Reject");
    rejectButton.addClickListener(e -> rejectRequest());
    
    form.addComponents(employee, customer, description, start, end, state, acceptButton, rejectButton);
    addComponent(form);
  }
  
  private void updateInfo(Optional<Trip> trip) {
    clearForm();
    acceptButton.setEnabled(false);
    rejectButton.setEnabled(false);
    if(trip.isPresent()) {
      employee.setValue(trip.get().getEmployeeEmail());
      customer.setValue(trip.get().getCustomer());
      if(trip.get().getDescription() != null)
      description.setValue(trip.get().getDescription());
      start.setValue(trip.get().getStart_date().toString());
      end.setValue(trip.get().getEnd_date().toString());
      state.setValue(trip.get().getState());
      
      if(state.getValue().equalsIgnoreCase("new")) {
        acceptButton.setEnabled(true);
        rejectButton.setEnabled(true);
      }
    }
  }
    
  private void clearForm() {
    employee.clear();
    customer.clear();
    description.clear();
    start.clear();
    end.clear();
    state.clear();
  }

  private void acceptRequest() {
    Set<Trip> selectedItems = grid.getSelectedItems();
    for(Trip t : selectedItems) {
      controller.acceptTrip(t, user);
      updateGrid();
      updateInfo(Optional.of(t));
    }
  }

  private void rejectRequest() {
    Set<Trip> selectedItems = grid.getSelectedItems();
    for(Trip t : selectedItems) {
      controller.rejectTrip(t, user);
      updateInfo(Optional.of(t));
    }
  }

  private void updateGrid() {
    List<Trip> trips = controller.getTripsByDepartment(user.getManaging());
    grid.setItems(trips);
  }
}
