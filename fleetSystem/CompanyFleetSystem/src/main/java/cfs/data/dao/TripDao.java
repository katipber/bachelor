package cfs.data.dao;

import cfs.data.entity.Employee;
import cfs.data.entity.Trip;
import java.time.LocalDate;
import java.util.List;
import java.util.Set;
import org.springframework.data.repository.CrudRepository;

/**
 * Trip data access object. The implementation of the interface is
 * generated automatically during the runtime.
 */

public interface TripDao extends CrudRepository<Trip, Long> {
  public List<Trip> findByEmployeeId(Long id);

  public List<Trip> findByEmployeeDepartmentId(Long id);

  public List<Trip> findByEnddateAfterAndStartdateBeforeAndEmployeeAndStateNot(LocalDate start_date, LocalDate end_date, Employee employee, String rejected);

}
