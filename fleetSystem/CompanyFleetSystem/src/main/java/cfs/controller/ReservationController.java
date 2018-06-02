package cfs.controller;

import cfs.data.dao.CarDao;
import cfs.data.dao.DepartmentDao;
import cfs.data.dao.EmployeeDao;
import cfs.data.dao.ReservationDao;
import cfs.data.dao.RoleDao;
import cfs.data.dao.TripDao;
import cfs.data.entity.Car;
import cfs.data.entity.Department;
import cfs.data.entity.Employee;
import cfs.data.entity.Reservation;
import cfs.data.entity.Role;
import cfs.data.entity.Trip;
import com.vaadin.server.VaadinService;
import java.time.LocalDate;
import java.util.LinkedList;
import java.util.List;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;

/**
 * This class implements the ReservationControllerInterface
 */

@Controller
public class ReservationController implements ReservationControllerInterface {
  /**
   * Autowired interface responsible for role persistence
   */
  @Autowired
  RoleDao         roleDao;
  /**
   * Autowired interface responsible for department persistence
   */
  @Autowired
  DepartmentDao   departmentDao;
  /**
   * Autowired interface responsible for employee persistence
   */
  @Autowired
  EmployeeDao     employeeDao;
  /**
   * Autowired interface responsible for trip persistence
   */
  @Autowired
  TripDao         tripDao;
  /**
   * Autowired interface responsible for reservation persistence
   */
  @Autowired
  ReservationDao  reservationDao;
  /**
   * Autowired interface responsible for car persistence
   */
  @Autowired
  CarDao          carDao;
    
  @Override
  public Employee login(String email, String pass) {
    List<Employee> employee = employeeDao.findByEmailAndPassword(email, pass);
    if(employee.isEmpty())
      return null;
    else
      return employee.get(0);
  }

  @Override
  public List<String> getRole(Employee user) {
    List<Role> roles = roleDao.findByEmployeeId(user.getId());

    List<String> roleNames = new LinkedList();
    for(Role r : roles)
      roleNames.add(r.getName());
    
    return roleNames;
  }

  @Override
  public List<Trip> getTrips(Employee user) {
    return tripDao.findByEmployeeId(user.getId());
  }

  @Override
  public boolean createTripRequest(Trip trip) {
    if(!validateTrip(trip)) return false;
    trip.setState("New");
    tripDao.save(trip);
    return true;
  }

  @Override
  public List<Trip> getTripsByDepartment(Department managing) {
    return tripDao.findByEmployeeDepartmentId(managing.getId());
  }

  @Override
  public List<Reservation> getReservations() {
    List<Reservation> expired = reservationDao.findByTripEnddateBeforeAndState(LocalDate.now(), "Open");
    for(Reservation r : expired) {
      r.setState("Expired");
      reservationDao.save(r);
    }
    return (List<Reservation>) reservationDao.findByCarModelNot("Dummy");
  }

  @Override
  public boolean startTrip(Reservation r) {
    if(r.getTrip().getStart_date().isAfter(LocalDate.now()))
      return false;
    r.setStart_date(LocalDate.now());
    r.setState("Started");
    reservationDao.save(r);
    return true;
  }

  @Override
  public boolean endTrip(Reservation reservation, Boolean damaged) {
    if(reservation.getStart_date() == null)
      return false;
    reservation.setEnd_date(LocalDate.now());
    reservation.setState("Ended");
    reservationDao.save(reservation);
    if(!damaged) return true;
    
    Car car = reservation.getCar();
    car.setState("Damaged");
    carDao.save(car);
    
    List<Reservation> openRes = reservationDao.findByCarAndState(car, "Open");
    for(Reservation r : openRes)
      makeReservation(r);
      
    return true;
  }

  @Override
  public void acceptTrip(Trip trip, Employee user) {
    trip.setManager(user);
    trip.setState("Accepted");
    tripDao.save(trip);
    
    if(trip.isReservation_request()) {
      Reservation reservation = new Reservation();
      reservation.setTrip(trip);
      reservation.setState("Open");
      makeReservation(reservation);
    }
  }

  @Override
  public void rejectTrip(Trip trip, Employee user) {
    trip.setManager(user);
    trip.setState("Rejected");
    tripDao.save(trip);
  }
  
  private boolean validateTrip(Trip trip) {
    if(trip.getCustomer().isEmpty() || trip.getStart_date() == null || trip.getEnd_date() == null) return false;
    if(trip.getStart_date().isBefore(LocalDate.now())) return false;
    if(trip.getEnd_date().isBefore(trip.getStart_date())) return false;
    List<Trip> result = tripDao.findByEnddateAfterAndStartdateBeforeAndEmployeeAndStateNot(trip.getStart_date(), trip.getEnd_date(), trip.getEmployee(), "Rejected");
    return result.isEmpty(); 
  }
  
  private void makeReservation(Reservation reservation) {
    List<Reservation> collision = reservationDao.findByTripEnddateAfterAndTripStartdateBefore(reservation.getTrip().getStart_date(), reservation.getTrip().getEnd_date());
    List<Car> cars = (List<Car>) carDao.findByState("OK");
    for(Reservation r : collision)
      cars.remove(r.getCar());

    if(cars.isEmpty())
      cars.add(carDao.findByModel("Dummy"));

    reservation.setCar(cars.get(0));
    reservationDao.save(reservation);
  }

}
