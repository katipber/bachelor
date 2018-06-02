package cfs.data.dao;

import cfs.data.entity.Employee;
import java.util.List;
import org.springframework.data.repository.CrudRepository;

/**
 * Employee data access object. The implementation of the interface is
 * generated automatically during the runtime.
 */

public interface EmployeeDao extends CrudRepository<Employee, Long> {
  List<Employee> findByEmailAndPassword(String email, String password);
}
