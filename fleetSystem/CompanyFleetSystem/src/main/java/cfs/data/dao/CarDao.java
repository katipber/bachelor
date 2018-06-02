package cfs.data.dao;

import cfs.data.entity.Car;
import java.util.List;
import org.springframework.data.repository.CrudRepository;

/**
 * Car data access object. The implementation of the interface is
 * generated automatically during the runtime.
 */

public interface CarDao extends CrudRepository<Car, Long> {
  public List<Car> findByState(String state);

  public Car findByModel(String model);
}
