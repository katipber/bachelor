package cfs.controller;

import cfs.data.entity.Department;
import cfs.data.entity.Employee;
import cfs.data.entity.Reservation;
import cfs.data.entity.Trip;
import java.util.List;
import java.util.Optional;

/**
 * Reservation controller interface specifies
 * all the necessary operations required for
 * the reservation business process
 */

public interface ReservationControllerInterface {
  /**
   * This method performs the login operation.
   * @param email user's email used for authentication
   * @param pass user's password used for authentication
   * @return the Employee object is returned if user exists
   */
  public Employee login(String email, String pass);
  
  /**
   * This method returns all of the roles of the specified user.
   * @param user an employee
   * @return list of roles
   */
  public List<String> getRole(Employee user);

  /**
   * This method returns all of the trips of the specified user.
   * @param user an employee
   * @return list of trips
   */
  public List<Trip> getTrips(Employee user);

  /**
   * This method returns all of the trips of the specified department.
   * @param managing the department
   * @return list of trips
   */
  public List<Trip> getTripsByDepartment(Department managing);

  /**
   * Returns the list of all reservations in the system
   * @return list of reservations
   */
  public List<Reservation> getReservations();

  /**
   * Creates a trip request in the system.
   * @param trip the trip object containing information about the trip
   * @return returns true if trip request was created otherwise fals
   */
  public boolean createTripRequest(Trip trip);

  /**
   * Accept the trip from the specified user
   * @param t the information about the trip
   * @param user an employee who requested the trip
   */
  public void acceptTrip(Trip t, Employee user);

  /**
   * Rejects the trip from the specified user
   * @param t the information about the trip
   * @param user an employee who requested the trip
   */
  public void rejectTrip(Trip t, Employee user);

  /**
   * Starts the realization of the trip
   * @param r reservation of the car
   * @return true if trip was started, otherwise false
   */
  public boolean startTrip(Reservation r);

  /**
   * Ends the realization of the trip
   * @param r reservation of the car
   * @param damaged true if the car is damaged, otherwisw false
   * @return true if the trip was ended, otherwise false
   */
  public boolean endTrip(Reservation r, Boolean damaged);
}
