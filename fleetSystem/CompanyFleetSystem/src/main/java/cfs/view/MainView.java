package cfs.view;

import cfs.data.entity.Employee;
import com.vaadin.navigator.View;
import com.vaadin.navigator.ViewChangeListener.ViewChangeEvent;
import com.vaadin.server.VaadinService;
import com.vaadin.spring.annotation.SpringView;
import com.vaadin.spring.annotation.UIScope;
import com.vaadin.ui.Alignment;
import com.vaadin.ui.Button;
import com.vaadin.ui.HorizontalLayout;
import com.vaadin.ui.Label;
import com.vaadin.ui.Panel;
import com.vaadin.ui.VerticalLayout;
import java.util.List;
import javax.annotation.PostConstruct;
import org.springframework.beans.factory.annotation.Autowired;
import cfs.controller.ReservationControllerInterface;

/**
 * The main view of the application. It is used to navigate
 * between all of the views based on the current user.
 */
@UIScope
@SpringView(name = MainView.PATH)
public class MainView extends VerticalLayout implements View {
  public static final String PATH = "main";

  @Autowired
  ReservationControllerInterface controller;
  Employee            user;
  Panel               panel;
  
  @PostConstruct
  protected void init() {}
  
  @Override
  public void enter(ViewChangeEvent event) {
    if(VaadinService.getCurrentRequest().getWrappedSession().getAttribute("user") == null)
      event.getNavigator().navigateTo("login");
    else if(user == null) {
      user = (Employee)VaadinService.getCurrentRequest().getWrappedSession().getAttribute("user");
      setLayout();
    }
  }

  private void setLayout() {
    HorizontalLayout menu = new HorizontalLayout();
    
    List<String> roles = controller.getRole(user);
    
    Button trips = new Button("My Trips");
    trips.addClickListener(e -> showTrips());
    menu.addComponent(trips);

    Button newTrip = new Button("New Trip");
    newTrip.addClickListener(e -> addNewTrip());
    menu.addComponent(newTrip);
    
    if(user.getManaging() != null) {
      Button requests = new Button("Requests");
      requests.addClickListener(e -> showRequests());
      menu.addComponent(requests);
    }

    if(roles.contains("FM")) {
      Button reservation = new Button("Reservations");
      reservation.addClickListener(e -> showReservations());
      menu.addComponent(reservation);
    }
    
    HorizontalLayout userBox = new HorizontalLayout();
    
    Label userName = new Label(user.getEmail());
    userBox.addComponent(userName);
    
    Button logout = new Button("LogOut");
    logout.addClickListener(e -> logoutSession());
    userBox.addComponent(logout);
    
    addComponents(userBox, menu);
    this.setComponentAlignment(userBox, Alignment.TOP_RIGHT);
    
    panel = new Panel();
    addComponent(panel);
    
    showTrips();
  }

  private void showTrips() {
    TripView view = new TripView(user, controller);
    panel.setContent(view);
  }

  private void addNewTrip() {
    NewTripView view = new NewTripView(user, controller);
    panel.setContent(view);
  }

  private void showRequests() {
    RequestView view = new RequestView(user, controller);
    panel.setContent(view);
  }

  private void showReservations() {
    ReservationView view = new ReservationView(user, controller);
    panel.setContent(view);
  }

  private void logoutSession() {
    VaadinService.getCurrentRequest().getWrappedSession().invalidate();
    getUI().getPage().setLocation("");
  }
  
}
